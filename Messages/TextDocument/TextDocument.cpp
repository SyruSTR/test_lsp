//
// Created by grusted on 6/21/25.
//


#include "TextDocument.h"
#include <iostream>
#include <regex>
#include <utility>
#include "fstream"
#include "../ResponseMessage.h"
#include "../NotificationMessage.h"
#include "TextDocumentSync.h"
#include "../RequestMessage.h"
#include "Params/DidOpenTextDocumentParams.h"
#include "Params/DocumentDiagnosticParams.h"
#include "Result/FullDocumentDiagnosticReport.h"
#include "../CompilerHandler/CompilerOutput.h"

#define GET_VALUE_FROM_JSON(variable, variable_name, json,variable_type) \
    if(json.contains(variable_name)) \
        variable = json[variable_name].get<variable_type>();


namespace  lsp_test {

    json get_message_without_params(int id) {
        json j = Message();
        j["id"] = id;
        j["params"] = nullptr;
        return j;
    }

    TextDocument::TextDocument(std::string *currentLine): m_currentReadedLine(currentLine) {
        m_textDocuments.clear();
        // m_dictionary = std::make_unique<DictionaryWords>("/usr/share/dict/words");
        m_dictionary = std::make_unique<DictionaryWords>();
        m_logger = std::make_unique<Logger>("/tmp/lsp-log.txt");
        m_compiler = std::make_unique<Compiler>("/tmp/compiler_log.txt");
    }

    TextDocument::~TextDocument() {
        if ( m_dictionary != nullptr )
            m_dictionary.reset();
        if (m_logger != nullptr)
            m_logger.reset();
    }

    void TextDocument::parseMessage(const std::string& raw_message) {
        auto message = nlohmann::json::parse(raw_message);

        m_logger->log(message.dump(),LogLevel::CLIENT);
        m_logger->log(message["method"].dump(),LogLevel::CLIENT);

        GET_VALUE_FROM_JSON(m_method,"method",message,typeof(m_method))

        if (m_method == "initialize") {
            initialize_response(message);
        }
        else if (m_method == "textDocument/completion") {
            completion(message);
        }
        else if (m_method == "textDocument/didChange") {
            didChange(message);
        }
        else if (m_method == "textDocument/diagnostic") {
            diagnostic(message);
        }
        else if (m_method == "textDocument/didOpen") {
            didOpen(message);
        }
        else if (m_method == "shutdown") {
            shutdown(message);
        }
        else if (m_method == "exit") {
            exit(message);
        }
    }

    void TextDocument::exit(const nlohmann::json &j) {

        //todo send error if get 'exit' before 'shutdown'
        exit(0);
    }

    void TextDocument::shutdown(const nlohmann::json& j) {
        int id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(id));

        //todo send error if request_method == 'exit'
        json response_json = get_message_without_params(id);

        m_logger->log(response_json.dump(), LogLevel::SERVER);
        sendResponse(response_json);

    }

    void TextDocument::completion(const nlohmann::json& j ) {
        int id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(int));

        //parse request from client
        auto request = RequestMessage<CompletionParams>(id,m_method,j["params"]);

        //get content from current file
        std::optional<std::string> content;
        if (m_textDocuments.contains(request.params->textDocument.uri))
            content = m_textDocuments.at(request.params->textDocument.uri);
        else {
            json response_json = get_message_without_params(id);
            sendResponse(response_json);
            return;
        }

        std::istringstream f(content.value());
        std::string currentLine;
        int i = 0;
        // get needed line for providing
        while (getline(f, *m_currentReadedLine, '\n')) {
            if (i == request.params->position.line)
                currentLine = *m_currentReadedLine;
            i++;
        }

        if (currentLine.empty()) {
            json response_json = get_message_without_params(id);
            sendResponse(response_json);
            return;
        }


        std::string lineUntilCursor = currentLine.substr(0, request.params->position.character);
        // extracts word after the last non-word character in the line
        std::regex pattern(".*\\W(.*?)");
        auto currentWord = std::regex_replace(lineUntilCursor, pattern, "$1");
        m_logger->log(currentLine+" | "+lineUntilCursor+" | "+currentWord, LogLevel::INFO);

        CompletionResult completionResult;

        // wrap possibly words for completion to completion items
        m_dictionary->WrapToCompletionList(completionResult.completion_list,currentWord);

        ResponseMessage<CompletionResult> response = ResponseMessage(request.id,completionResult);

        nlohmann::json response_json = response;
        m_logger->log(response_json.dump(), LogLevel::SERVER);
        sendResponse(response_json);

    }

    void TextDocument::diagnostic(const nlohmann::json& j) {
        int id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(int));

        auto request = RequestMessage<DocumentDiagnosticParams>(id,m_method,j["params"]);

        //get content from current file
        std::optional<std::string> content;
        if (m_textDocuments.contains(request.params->textDocument.uri))
            content = m_textDocuments.at(request.params->textDocument.uri);
        else {
            json response_json = get_message_without_params(id);
            sendResponse(response_json);
            return;
        }

        // get all words from current file
        // auto wordsInDocument = resplit(content.value(),std::regex("\\W"));
        // // split content for lines
        // auto lines = resplit(content.value(),std::regex("\n"));
        // //erase empty words
        // std::erase_if(wordsInDocument,
        //               [this](const std::string& line) {
        //                   return line.empty();
        //               });

        auto report = FullDocumentDiagnosticReport();
        std::string tmp_str = m_compiler->run(content.value());
        if (!tmp_str.empty()) {
            // json response_json = get_message_without_params(id);
            // sendResponse(response_json);
            compiler_output _comp_output = json::parse(tmp_str);
            report.items.push_back({
                Diagnostic{
                    Range{
                        Position(_comp_output.line,_comp_output.char_position),
                        Position(_comp_output.line, _comp_output.char_position+3),
                    },
                    ERROR,
                    _comp_output.error_message,
                }
            });
        }



        // std::vector<std::string> invalidWords;
        //
        // for (std::string& word : wordsInDocument) {
        //     if (!m_dictionary->Contains(word)) {
        //         // get word
        //         // \b means empty space
        //         std::regex pattern("\\b"+word+"\\b");
        //         for (int i=0; i < lines.size(); i++) {
        //             //looking wrong word in lines
        //             auto matches = std::sregex_iterator(lines[i].begin(),lines[i].end(),pattern);
        //             auto matches_end = std::sregex_iterator();
        //             for (std::sregex_iterator match = matches; match != matches_end; ++match) {
        //                 report.items.push_back({
        //                     Diagnostic{
        //                         Range{
        //                             Position(i,match->position()),
        //                             Position(i,match->position()+word.length()),
        //                         },
        //                         ERROR,
        //                         word + " is not in our dictionary"
        //                     }
        //                 });
        //             }
        //         }
        //     }
        //
        // }

        //hardcode
        nlohmann::json json_response = Message();
        json_response["id"] = id;
        json_response["result"] = report;
        //end of hardcode

        m_logger->log(json_response.dump(),LogLevel::SERVER);
        sendResponse(json_response);
    }

    std::vector<std::string> TextDocument::resplit(const std::string &s, const std::regex &sep_regex = std::regex{"\\s+"}) {
        std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
        std::sregex_token_iterator end;
        return {iter, end};
    }

    void TextDocument::sendResponse(const nlohmann::json& response) {
        std::cout << "Content-Length: " << response.dump().length() << "\r\n\r\n" << response.dump();
        //debug message
        std::cerr << response.dump() << std::endl;
        std::cout.flush();
    }

    void TextDocument::didChange(const nlohmann::json& j) {
        DidChangeTextDocumentParams params;
        GET_VALUE_FROM_JSON(params,"params",j,typeof(params))

        auto notification = NotificationMessage<DidChangeTextDocumentParams>(m_method, params);

        // add filepath to map and text from file
        if (notification.params.has_value())
            m_textDocuments[notification.params.value().textDocument.uri] = notification.params.value().contentChange[0].text;

        const nlohmann::json json_notification;
        m_logger->log(json_notification.dump(),LogLevel::CLIENT);
    }

    void TextDocument::didOpen(const nlohmann::json &j) {
        DidOpenTextDocumentParams params;
        GET_VALUE_FROM_JSON(params,"params",j,typeof(params))

        auto notification = NotificationMessage<DidOpenTextDocumentParams>(m_method, params);

        if (notification.params.has_value())
            m_textDocuments[notification.params.value().textDocument.uri] = notification.params.value().textDocument.text;

        const nlohmann::json json_notification;
        m_logger->log(json_notification.dump(),LogLevel::CLIENT);
    }


    void TextDocument::initialize_response(const nlohmann::json& j) {
        int id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(id));

        auto result = InitializerResult(
            ServerInfo(),
             [] {
                 //TODO json -> ServerCapabilities
                 nlohmann::json serverCapabilities;
                 serverCapabilities["completionProvider"] =
                         nlohmann::json::object();
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
        auto msg = ResponseMessage(id,result);
        const nlohmann::json response = msg;
        m_logger->log(response.dump(),LogLevel::SERVER);
        sendResponse(response);
    }

}