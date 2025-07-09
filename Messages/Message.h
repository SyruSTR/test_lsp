//
// Created by grusted on 6/16/25.
//

#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <utility>
#include "../external/json.hpp"

namespace lsp_test {
    using json = nlohmann::json;

    struct Message {
        std::string jsonrpc;

        explicit Message(std::string jsonrpc) : jsonrpc(std::move(jsonrpc)) {}
        Message() {
            this->jsonrpc = "2.0";
        }
    };

    void inline to_json(json &j, const Message &msg) {
        j["jsonrpc"] = msg.jsonrpc;
    }

    void inline from_json(const json &j, Message &msg) {
        j.at("jsonrpc").get_to(msg.jsonrpc);
    }
}

#endif //MESSAGE_H
