//
// Created by grusted on 6/16/25.
//

#ifndef RESPONSEMESSAGE_H
#define RESPONSEMESSAGE_H

#include "Message.h"
namespace lsp_test {

    using json = nlohmann::json;

    struct ResponseMessage : Message {
        int64_t id;
        json result;

        ResponseMessage(const std::string &jsonrpc, int64_t id) : Message(jsonrpc), id(id) {}
        ResponseMessage(const int64_t id) : id(id) {
            this->jsonrpc = Message().jsonrpc;
        }

        void to_json(json &j, const ResponseMessage &msg) {
            j = json{
                {"jsonrpc",msg.jsonrpc},
                {"id",msg.id},
                {"result",result}
            };
        }
    };
}

#endif //RESPONSEMESSAGE_H
