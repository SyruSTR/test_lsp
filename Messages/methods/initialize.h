//
// Created by grusted on 6/16/25.
//

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "../RequestMessage.h"
#include "../ResponseMessage.h"
#include "../../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    json initialize_responce(const ResponseMessage &msg) {
        json response = msg;
        ServerInfo server_info {"my-lsp-server","0.0.1"};
        response["result"]["capabilities"]["completionProvider"] = json::object();
        return response;
    }
}

#endif //INITIALIZE_H
