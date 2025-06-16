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

int main() {
    waitForDebugger();
    Logger logger("/tmp/lsp-log.txt");

    std::string header;
    std::string line;
    int contentLength = 0;

    // std::cerr << "LSP start! GGWP!" << std::endl;

    while (std::getline(std::cin, line)) {
        if (line == "\r" || line.empty()) {
            if (contentLength > 0) {
                std::string body(contentLength, '\0');
                std::cin.read(&body[0], contentLength);



                auto message = json::parse(body);
                logger.log(message.dump(),LogLevel::CLIENT);
                // logger.log(message["id"].dump(),LogLevel::CLIENT);
                logger.log(message["method"].dump(),LogLevel::CLIENT);

                std::cerr << "Got message!" << std::endl;
                // logger.log(message["method"].get<std::string>(),LogLevel::WARNING);
                auto method = message["method"].get<std::string>();
                if (method == "initialize") {
                    int id = message["id"].get<int>();
                    auto msg = lsp_test::ResponseMessage(id);

                    json response = lsp_test::initialize_responce(msg);
                    logger.log(response.dump(),LogLevel::SERVER);
                    sendResponse(response);
                }
                else if (method == "textDocument/completion") {
                    int id = message["id"].get<int>();
                    auto msg = lsp_test::ResponseMessage(id);
                    lsp_test::CompletionList completion;
                    completion.isIncomplete = false;
                    json response = lsp_test::completion(msg, completion);
                    logger.log(response.dump(),LogLevel::SERVER);
                    sendResponse(response);
                }

                // TODO: Parse JSON and respond accordingly
                // std::cerr << "Received: " << body << std::endl;



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
