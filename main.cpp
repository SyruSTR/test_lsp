//
// Created by grusted on 6/14/25.
//
#include <iostream>
#include <string>
#include <regex>
#include <csignal>
#include <unistd.h>
#include <fstream>
#include "external/json.hpp"
using json = nlohmann::json;


void waitForDebugger() {
    raise(SIGSTOP);
}

void sendResponse(const json& response) {
    std::cout << "Content-Length: " << response.dump().length() << "\r\n\r\n" << response.dump();
    std::cout.flush();
}

int main() {

    waitForDebugger();
    std::string header;
    std::string line;
    int contentLength = 0;

    std::cerr << "LSP start! GGWP!" << std::endl;

    while (std::getline(std::cin, line)) {
        if (line == "\r" || line.empty()) {
            if (contentLength > 0) {
                std::string body(contentLength, '\0');
                std::cin.read(&body[0], contentLength);

                // TODO: Parse JSON and respond accordingly
                std::cerr << "Received: " << body << std::endl;

                // example responce 'init'
                json response = R"({
                    "jsonrpc": "2.0",
                    "id": 1,
                    "result": {
                        "capabilities": {}
                    }
                })";
                sendResponse(response);

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
