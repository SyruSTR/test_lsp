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
#include "Rapams/DocumentDiagnosticParams.h"

#define GET_VALUE_FROM_JSON(variable, variable_name, json,variable_type) \
    if(json.contains(variable_name)) \
        variable = json[variable_name].get<variable_type>();


namespace  lsp_test {

    TextDocument::TextDocument(std::string *currentLine): m_currentReadedLine(currentLine) {
        m_textDocuments.clear();
        m_dictionary = std::make_unique<DictionaryWords>("/usr/share/dict/words");
        m_logger = std::make_unique<Logger>("/tmp/lsp-log.txt");
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
    }


    void TextDocument::completion(const nlohmann::json& j ) {
        int id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(int));


        auto request = RequestMessage<CompletionParams>(id,m_method,j["params"]);
        std::optional<std::string> content;
        if (m_textDocuments.contains(request.params->textDocument))
            content = m_textDocuments.at(request.params->textDocument);
        else {
            return;
        }

        std::istringstream f(content.value());
        std::string currentLine;
        int i = 0;
        while (getline(f, *m_currentReadedLine, '\n')) {
            if (i == request.params->position.line)
                currentLine = *m_currentReadedLine;
            i++;
        }

        if (currentLine.empty())
            return;

        std::string lineUntilCursor = currentLine.substr(0, request.params->position.character);
        std::regex pattern(".*\\W(.*?)");
        auto currentWord = std::regex_replace(lineUntilCursor, pattern, "$1");
        //TODO add words filter
        m_logger->log(currentLine+" | "+lineUntilCursor+" | "+currentWord, LogLevel::INFO);

        CompletionResult completionResult;

        m_dictionary->WrapToCompletionList(completionResult.completion_list,currentWord);
        completionResult.completion_list.isIncomplete = true;
        ResponseMessage<CompletionResult> response = ResponseMessage(request.id,completionResult);

        nlohmann::json response_json = response;
        m_logger->log(response_json.dump(), LogLevel::SERVER);
        sendResponse(response_json);

    }

    void TextDocument::diagnostic(const nlohmann::json& j) {
        int id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(int));
        auto request = RequestMessage<DocumentDiagnosticParams>(id,m_method,j["params"]);


        //hardcode
        nlohmann::json json_response = Message();
        json_response["id"] = id;
        json_response["result"] = request;
        //end of hardcode

        m_logger->log(json_response.dump(),LogLevel::SERVER);
        sendResponse(json_response);
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

        if (notification.params.has_value())
            m_textDocuments[notification.params.value().textDocument] = notification.params.value().contentChange[0].text;

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