//
// Created by grusted on 6/16/25.
//

#ifndef RESPONSEMESSAGE_H
#define RESPONSEMESSAGE_H

#include "Message.h"
#include "methods/ServerInfo.h"

namespace lsp_test {

    using json = nlohmann::json;

    struct ResponseMessage : Message {
        int64_t id;
        json result;

        ResponseMessage(const std::string &jsonrpc, int64_t id) : Message(jsonrpc), id(id) {
            result["result"].push_back({"serverInfo",ServerInfo()});
        }
        ResponseMessage(const int64_t id) : id(id) {
            auto info = ServerInfo();
            result["result"].push_back({"serverInfo",ServerInfo()});
            this->jsonrpc = Message().jsonrpc;
        }
    };

    void to_json(json &j, const ResponseMessage &msg) {
        j = json{
                {"jsonrpc", msg.jsonrpc},
                {"id", msg.id},
                {"result", msg.result}
        };
    }

}


#endif //RESPONSEMESSAGE_H
