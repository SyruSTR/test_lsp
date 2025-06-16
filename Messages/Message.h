//
// Created by grusted on 6/16/25.
//

#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "../external/json.hpp"
using json = nlohmann::json;

struct Message {
    std::string jsonrpc;

    Message(const std::string &jsonrpc) : jsonrpc(jsonrpc) {}
    Message() {
        this->jsonrpc = "2.0";
    }

    void to_json(json &j, const Message &msg) {
        j = json({"jsonrpc",msg.jsonrpc});
    }

    void from_json(const json &j, Message &msg) {
        j.at("jsonrpc").get_to(msg.jsonrpc);
    }
};

#endif //MESSAGE_H
