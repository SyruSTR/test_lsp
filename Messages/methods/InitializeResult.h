//
// Created by grusted on 6/18/25.
//

#ifndef INITIALIZERESULT_H
#define INITIALIZERESULT_H
#include "ServerInfo.h"
#include "../../external/json.hpp"
#include "../ResponceResult.h"

namespace lsp_test {

    using json = nlohmann::json;

    struct InitializerResult : ResponseResult {
        //todo make own struct
        json capabilities;
        ServerInfo serverInfo;

        InitializerResult(const ServerInfo& server_info, const json& capabilities) : ResponseResult(), serverInfo(server_info), capabilities(capabilities) {}
    };

    void to_json(json& j, const InitializerResult& result) {
        j["capabilities"] = result.capabilities;
        j["serverInfo"] = result.serverInfo;
    }

}

#endif //INITIALIZERESULT_H
