//
// Created by grusted on 6/14/25.
//
#include <iostream>
#include <string>
#include <regex>
#include <csignal>
#include <fstream>
#include "Messages/TextDocument/TextDocument.h"
using json = nlohmann::json;


#ifdef DEBUG
void waitForDebugger() {
    raise(SIGSTOP);
}
#endif

int main() {
#ifdef DEBUG
    waitForDebugger();
#else
    std::cerr << "release mode" << std::endl;
#endif


    std::string header;
    std::string line;
    int contentLength = 0;

    //send reference to current line from stdin
    lsp_test::TextDocument textDocument(&line);

    while (std::getline(std::cin, line)) {
        if (line == "\r" || line.empty()) {
            if (contentLength > 0) {
                std::string body(contentLength, '\0');
                std::cin.read(&body[0], contentLength);

                textDocument.parseMessage(body);

                contentLength = 0;
            }
        } else {
            std::smatch match;
            // parse the header from client request or notification
            if (std::regex_search(line, match, std::regex("Content-Length: (\\d+)"))) {
                contentLength = std::stoi(match[1]);
            }
        }
    }

    return 0;
}
