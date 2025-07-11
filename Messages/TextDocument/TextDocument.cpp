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

    json get_message_without_params(int64_t id) {
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
        int64_t id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(id));

        //todo send error if request_method == 'exit'
        json response_json = get_message_without_params(id);

        m_logger->log(response_json.dump(), LogLevel::SERVER);
        sendResponse(response_json);

    }

    void TextDocument::completion(const nlohmann::json& j ) {
        int64_t id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(int64_t));

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
        int64_t i = 0;
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
        int64_t id = 0;
        GET_VALUE_FROM_JSON(id,"id",j,typeof(int64_t));

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
        // std::string tmp_str = m_compiler->run(content.value(), true);
        if (!tmp_str.empty()) {
            // json response_json = get_message_without_params(id);
            // sendResponse(response_json);
            try {
                CompilerOutput _comp_output = json::parse(tmp_str);

     //            report.items.push_back({
     //     Diagnostic{
     //         Range{
     //             Position(_comp_output.line.value(),_comp_output.char_pos.value()),
     //             Position(_comp_output.line.value(),
     //                      _comp_output.char_pos.value() + (_comp_output.token_content.has_value() ? _comp_output.token_content.value().length() : 1 )),
     //         },
     //         ERROR,
     //         _comp_output.message.value(),
     //     }
     // });
                std::string message_buffer = _comp_output.message.value_or("Without message: ");
                if (_comp_output.message.has_value() && _comp_output.message.value() == "Unresolved error") {
                    report.items.emplace_back(
                             Range{
                                 Position(_comp_output.location.value().line,_comp_output.location.value().char_pos),
                                 Position(_comp_output.location.value().line,_comp_output.location.value().char_pos),
                             },
                             ERROR,
                             "Unresolved error"
                         );
                }
                else {
                    Range range_buffer;
                    std::vector<std::string> lines = resplit(content.value(),std::regex("\n"));
                    __gnu_cxx::__normal_iterator<std::string *, std::vector<std::string>> current_line;

                    if (_comp_output.location.has_value())
                        current_line = lines.begin()+_comp_output.location.value().line;
                    switch (_comp_output.error_code) {
                        case ER_LEX:
                            message_buffer = _comp_output.message.value_or("Without message: ");
                            range_buffer.start = Position(_comp_output.location.value().line,_comp_output.location.value().char_pos);
                            range_buffer.end = range_buffer.start;
                            range_buffer.end.character++;
                            break;
                        case ER_SYNTAX: {
                            if (_comp_output.token_error.has_value()) {
                                int64_t token_length = _comp_output.token_error.value().token.get_token_length();

                                range_buffer.start = Position(_comp_output.location.value().line,_comp_output.location.value().char_pos);
                                range_buffer.end = Position(_comp_output.location.value().line,_comp_output.location.value().char_pos + token_length);

                                if (_comp_output.token_error.has_value())
                                    message_buffer += to_string(_comp_output.token_error.value().token.type);

                                // report.items.emplace_back(
                                //  Range{
                                //      Position(_comp_output.line.value_or(0),_comp_output.char_pos.value_or(1)),
                                //      Position(_comp_output.line.value_or(0),_comp_output.char_pos.value_or(1) + token_length),
                                //  },
                                //  ERROR,
                                //  _comp_output.message.value_or("Without message") + _comp_output.token->token_type.get_token_string_representation()
                                //  );
                            }
                            break;
                        }
                        case ER_UNDEF_FUNC_OR_REDEF_VAR: {
                            std::string id_name = _comp_output.token_error.value().token.token_string;
                            int64_t start = 0;
                            int64_t end = 0;
                            // if wanted call function, but it's variable
                            if (_comp_output.token_error.value().token.type == T_BRACKET_OPEN) {
                                start = current_line->find(id_name);
                            }
                            else {
                                start = _comp_output.location.value().char_pos;
                            }
                            end = start + id_name.length();

                            range_buffer.start = Position(_comp_output.location.value().line, start);
                            range_buffer.end = Position(_comp_output.location.value().line, end);

                            message_buffer = "redefinition of variable: '" + id_name + "'";
                            break;
                        }
                        case ER_PARAMS_ARGS_MISMATCH:
                        case ER_PARAMS_TYPE_MISMATCH: {
                            //get func call brackets positions for highlighting
                            int64_t start = current_line->find('(');
                            if (start == -1)
                                start = 0;
                            int64_t end = current_line->find(')');
                            if (end == -1)
                                end = current_line->length();

                            // it's necessary to add +1 to get correct highlighting
                            end++;
                            range_buffer.start = Position(_comp_output.location.value().line, start);
                            range_buffer.end = Position(_comp_output.location.value().line, end);

                            if (_comp_output.error_code == ER_PARAMS_TYPE_MISMATCH) {
                                if (_comp_output.func_type_mismatch.has_value()) {
                                    auto is_nil_possibility_string = [](ItemType const type ,bool const is_nil_possibility) {
                                        return is_nil_possibility && type != IT_NIL  ? "?" : "";
                                    };
                                    message_buffer = "function '" + _comp_output.func_type_mismatch.value().func.name + "' expect type '"
                                                                + to_string(_comp_output.func_type_mismatch.value().types.expected_type)
                                    + is_nil_possibility_string(_comp_output.func_type_mismatch.value().types.expected_type,
                                        _comp_output.func_type_mismatch.value().expected_is_nil_possibility)
                                    + "' but has got '"
                                                                + to_string(_comp_output.func_type_mismatch.value().types.actual_type)
                                    + is_nil_possibility_string(_comp_output.func_type_mismatch.value().types.actual_type,
                                        _comp_output.func_type_mismatch.value().actual_is_nil_possibility) + "'";
                                }
                            }
                            else {
                                if (_comp_output.argument_mismatch.has_value()) {
                                    message_buffer = "function has " + std::to_string(_comp_output.argument_mismatch.value().expected_func_args) + " parameters"
                                                                + ", but is called with " + std::to_string(_comp_output.argument_mismatch.value().actual_func_args) + " argument"
                                                                + (_comp_output.argument_mismatch.value().actual_func_args > 1 ? "s" : "");
                                }
                            }
                            break;
                        }
                        case ER_UNDEF_VAR_OR_NOTINIT_VAR:
                        {
                            std::string var_name = _comp_output.variable_info.value().variable_name;
                            int64_t var_length = var_name.length();

                            // std::vector<std::string> lines = resplit(content.value(),std::regex("\n"));
                            // auto current_line = lines.begin()+_comp_output.line.value();

                            int64_t start = 0;
                            int64_t end = 0;
                            if (_comp_output.variable_info.value().is_it_assigment) {
                                start = _comp_output.location.value().char_pos;
                                end = start + var_length;
                            }
                            else {
                                start = std::max(_comp_output.location.value().char_pos - var_length,static_cast<int64_t>(0));
                                end = std::min(_comp_output.location.value().char_pos, static_cast<int64_t>(current_line->length()));
                                int64_t whitespaces_count = 0;
                                for (auto it = current_line->begin() + static_cast<int64_t>(current_line->find(var_name)); it != current_line->begin()+end; ++it ) {
                                    if (std::isspace(*it)) {
                                        whitespaces_count++;
                                    }
                                }
                                start -= whitespaces_count;
                                end -= whitespaces_count;
                            }

                            range_buffer.start = Position(_comp_output.location.value().line, start);
                            range_buffer.end = Position(_comp_output.location.value().line, end);

                            message_buffer = "Variable: " + var_name + " Undefined or non initialize";
                            // std::string message = "Variable: " + var_name + " Undefined or non initialize";

                            // report.items.emplace_back(
                            // Range{
                            //      Position(_comp_output.line.value_or(0),start),
                            //      Position(_comp_output.line.value_or(0),end),
                            // },
                            // ERROR,
                            // message
                            // );
                            break;
                        }
                        case ER_FUNC_RETURN: {
                            std::string str_return = "return";
                            int64_t start = current_line->find(str_return);
                            std::cerr << start << std::endl;
                            if (start == -1)
                                start = 0;
                            int64_t end = start + str_return.length();

                            range_buffer.start = Position(_comp_output.location->line,start);
                            range_buffer.end = Position(_comp_output.location->line,end);

                            if (_comp_output.return_info.has_value()) {
                                if (_comp_output.return_info.value().is_void_function)
                                    message_buffer = "A function with void return type shouldn't return a value";
                                else
                                    message_buffer = "A function with non-void return type should return a value";
                            }

                            break;
                        }
                        case ER_TYPE_COMP:
                            break;
                        case ER_INFERENCE:
                            break;
                        case ER_OTHER_SEM:
                            break;
                        case ER_INTERNAL:
                            range_buffer.start = Position(0,0);
                            range_buffer.end = Position(lines.capacity(),lines.end()->length());
                            // report.items.emplace_back(
                            //     Range{
                            //         Position(0,0),
                            //         Position(lines.capacity(),lines.end()->capacity())
                            //     },
                            //     ERROR,
                            //     _comp_output.message.value_or("Without message")
                            //     );
                            break;
                    }
                    report.items.emplace_back(
                        range_buffer,
                        ERROR,
                        message_buffer);
                }
            }
            catch (json::parse_error& e) {
                std::cerr << e.what() << std::endl;
            }
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
        int64_t id = 0;
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