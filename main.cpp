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
                logger.log(message.dump(),LogLevel::WARNING);
                logger.log(message["id"].dump(),LogLevel::CLIENT);
                logger.log(message["method"].dump(),LogLevel::CLIENT);

                std::cerr << "Got message!" << std::endl;
                // logger.log(message["method"].get<std::string>(),LogLevel::WARNING);
                std::string method = message["method"].get<std::string>();
                if (method == "initialize") {
                    int id = message["id"].get<int>();
                    // example responce 'init'
                    // json response = R"({
                    //     "jsonrpc": "2.0",
                    //     "id": 1,
                    //     "serverInfo": {
                    //         "name": "my-lsp-server",
                    //         "version": "0.0.1",
                    //     },
                    // })"_json;
                    json response = {
                        {"jsonrpc","2.0"},
                        {"id",id},
                        {"serverInfo",{
                            {"name","my-lsp-server"},
                            {"version", "0.0.1"},
                        }}
                    };
                    response["result"]["capabilities"]["completionProvider"] = json::object();
                    logger.log(response.dump(),LogLevel::SERVER);
                    sendResponse(response);
                }
                // else if (method == "textDocument/completion") {
                //     int id = message["id"].get<int>();
                //     json response = {
                //         {"jsonrpc","2.0"},
                //         {"id",id},
                //         {"serverInfo",{
                //                 {"name","my-lsp-server"},
                //                 {"version", "0.0.1"},
                //             }}
                //     };
                //     response["result"]["capabilities"]["completionProvider"] = json::object();
                //     CompletionList completion;
                //     // response["result"]["CompletionList"] = completion.test_CompletionList();
                //     logger.log(response.dump(),LogLevel::SERVER);
                //     sendResponse(response);
                // }

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
