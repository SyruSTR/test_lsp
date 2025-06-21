//
// Created by grusted on 6/14/25.
//
#include <iostream>
#include <string>
#include <regex>
#include <csignal>
#include <unistd.h>
#include <fstream>
// #include "external/json.hpp"
#include "Logger/Logger.h"
#include "Messages/TextDocument/Completion.h"
#include "Messages/methods/initialize.h"
#include "Messages/ResponseMessage.h"
#include "Messages/TextDocument/DidChange.h"
#include "Documents.h"
#include "Messages/DictionaryWords.h"
#include "Messages/TextDocument/CompletionParams.h"
#include "Messages/TextDocument/Diagnostic.h"
#include "Messages/TextDocument/DocumentDiagnosticParams.h"
using json = nlohmann::json;


void waitForDebugger() {
    raise(SIGSTOP);
}

void sendResponse(const json& response) {
    std::cout << "Content-Length: " << response.dump().length() << "\r\n\r\n" << response.dump();
    //debug message
    std::cerr << response.dump() << std::endl;
    std::cout.flush();
}

std::vector<std::string> resplit(const std::string &s, const std::regex &sep_regex = std::regex{"\\s+"}) {
    std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    std::sregex_token_iterator end;
    return {iter, end};
}

int main() {
    waitForDebugger();
    Logger logger("/tmp/lsp-log.txt");

    std::string header;
    std::string line;
    int contentLength = 0;

    lsp_test::Documents documents;

    DictionaryWords dictionaryWords;

    dictionaryWords.AddWordsFromFile("/usr/share/dict/words");

    while (std::getline(std::cin, line)) {
        if (line == "\r" || line.empty()) {
            if (contentLength > 0) {
                std::string body(contentLength, '\0');
                std::cin.read(&body[0], contentLength);



                auto message = json::parse(body);
                logger.log(message.dump(),LogLevel::CLIENT);
                logger.log(message["method"].dump(),LogLevel::CLIENT);


                std::cerr << "Got message!" << std::endl;
                auto method = message["method"].get<std::string>();
                if (method == "initialize") {
                    int id = message["id"].get<int>();
                    lsp_test::InitializerResult result = lsp_test::InitializerResult(lsp_test::ServerInfo(),
                    [] {
                             //TODO json -> ServerCapabilities
                             json serverCapabilities;
                             serverCapabilities["completionProvider"] = json::object();
                             serverCapabilities["textDocumentSync"] = FULL;
                            serverCapabilities["diagnosticProvider"] = {
                                {
                                    "interFileDependencies", false
                                },
                                {
                                    "workspaceDiagnostics", false
                                }
                            };
                             return serverCapabilities;
                         }()
                        );
                    auto msg = lsp_test::ResponseMessage(id,result);

                    json response = lsp_test::initialize_response(msg);
                    logger.log(response.dump(),LogLevel::SERVER);
                    sendResponse(response);
                }
                else if (method == "textDocument/completion") {
                    int id = message["id"].get<int>();
                    auto msg = lsp_test::RequestMessage<lsp_test::CompletionParams>(id,method,message["params"]);

                    std::optional<std::string> content;
                    if (documents.textDocuments.contains(msg.params->textDocument))
                        content = documents.textDocuments.at(msg.params->textDocument);
                    else {
                        contentLength = 0;
                        continue;
                    }
                    std::istringstream f(content.value());
                    std::string currentLine;
                    int i = 0;
                    while (getline(f, line, '\n')) {
                        if (i == msg.params->position.line)
                            currentLine = line;
                        i++;
                    }
                    if (currentLine.empty()) {
                        contentLength = 0;
                        continue;
                    }

                    std::string lineUntilCursor = currentLine.substr(0, msg.params->position.character);

                    std::regex pattern(".*\\W(.*?)");
                    auto currentWord = std::regex_replace(lineUntilCursor, pattern, "$1");
                    //TODO add words filter
                    logger.log(currentLine+" | "+lineUntilCursor+" | "+currentWord, LogLevel::INFO);

                    // переписать на класс, а то это уже не смешно(
                    json response = lsp_test::completion(msg,dictionaryWords, currentWord);
                    logger.log(response.dump(),LogLevel::SERVER);
                    sendResponse(response);
                }
                else if (method == "textDocument/didChange") {
                    lsp_test::DidChangeTextDocumentParams params = message["params"];
                    auto notification = lsp_test::NotificationMessage<lsp_test::DidChangeTextDocumentParams>(method,params);
                    json request = lsp_test::DidChange(notification);

                    if (notification.params.has_value())
                        documents.textDocuments[notification.params.value().textDocument] = notification.params.value().contentChange[0].text;
                    logger.log(request.dump(),LogLevel::CLIENT);
                }
                else if (method == "textDocument/diagnostic") {

                    int id = message["id"].get<int>();

                    auto request = lsp_test::RequestMessage<lsp_test::DocumentDiagnosticParams>(id,method,message["params"]);


                    // std::optional<std::string> content;
                    // if (documents.textDocuments.contains(msg.params->textDocument))
                    //     content = documents.textDocuments.at(msg.params->textDocument);
                    // else {
                    //     contentLength = 0;
                    //     continue;
                    // }
                    //
                    // auto wordsInDocument = resplit(content.value(),std::regex("\\W"));
                    //
                    // vector<std::string> invalidWords;
                    //
                    // for (auto std::string& word : wordsInDocument) {
                    //
                    // }

                    auto report = lsp_test::diagnostic<lsp_test::DocumentDiagnosticParams>(request);
                    //hardcode
                    json j;
                    j = lsp_test::Message();
                    j["id"] = id;
                    // j["method"] = message["method"].get<std::string>();
                    j["result"] = report;
                    //end of hardcode
                    // to_json(j,report);
                    logger.log(j.dump(),LogLevel::SERVER);
                    sendResponse(j);
                }

                // TODO: Parse JSON and respond accordingly

                contentLength = 0;
            }
        } else {
            std::smatch match;
            if (std::regex_search(line, match, std::regex("Content-Length: (\\d+)"))) {
                contentLength = std::stoi(match[1]);
            }
        }
    }

    return 0;
}
