//
// Created by grusted on 6/16/25.
//

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "../RequestMessage.h"
#include "InitializeResult.h"
#include "../ResponseMessage.h"

namespace lsp_test {

    using json = nlohmann::json;

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

    json initialize_responce(const ResponseMessage &msg) {
        json response = msg;
        ServerInfo server_info {"my-lsp-server","0.0.1"};
        response["serverInfo"] = {
        {"name",server_info.name},
        {"version",server_info.version},
        };
        response["result"]["capabilities"]["completionProvider"] = json::object();
        return response;
    }
}

#endif //INITIALIZE_H
