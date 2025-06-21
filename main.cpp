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


void waitForDebugger() {
    raise(SIGSTOP);
}

std::vector<std::string> resplit(const std::string &s, const std::regex &sep_regex = std::regex{"\\s+"}) {
    std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    std::sregex_token_iterator end;
    return {iter, end};
}

int main() {
    waitForDebugger();

    std::string header;
    std::string line;
    int contentLength = 0;

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
            if (std::regex_search(line, match, std::regex("Content-Length: (\\d+)"))) {
                contentLength = std::stoi(match[1]);
            }
        }
    }

    return 0;
}
