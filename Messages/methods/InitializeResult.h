//
// Created by grusted on 6/16/25.
//

#ifndef INITIALIZERESULT_H
#define INITIALIZERESULT_H

#include <map>
#include <optional>

#include "ServerInfo.h"

template <typename T>
struct InitializeResult {
    std::optional<lsp_test::ServerInfo> serverInfo;
    std::map<std::string, T> Capabilities;

};

#endif //INITIALIZERESULT_H
