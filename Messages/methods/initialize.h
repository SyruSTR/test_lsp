//
// Created by grusted on 6/16/25.
//

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "../RequestMessage.h"
#include "InitializeResult.h"

template <typename T, typename R>
struct initialize {
     InitializeResult<R> initialize(RequestMessage<T> request) {
         InitializeResult<R> initialize_result;
         ServerInfo server_info;
         server_info.name = "Test LSP server";
         server_info.version = "0.0.1";
         initialize_result.Capabilities = new std::map<std::string, R>;
         initialize_result.serverInfo = server_info;
        return initialize_result;
    }
};

#endif //INITIALIZE_H
