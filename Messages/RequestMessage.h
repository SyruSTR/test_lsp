//
// Created by grusted on 6/16/25.
//

#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H

#include <optional>
#include "Message.h"
namespace lsp_test {
    template <typename T>
struct RequestMessage:Message {

        int64_t id;
        std::string method;
        std::optional<T> params; //something else

        RequestMessage(
            uint64_t id,
            std::string method,
            std::optional<T> params,
            std::string jsonrpc) :
        Message(jsonrpc), id(id), method(method), params(params) {}

        RequestMessage(
            uint64_t id,
            std::string method,
            std::string jsonrpc) :
        Message(jsonrpc), id(id), method(method), params(params) {
            this.params = std::nullopt;
        }

        RequestMessage(
            uint64_t id,
            std::string method) :
        id(id), method(method), params(params) {
            this->jsonrpc = Message();
            this.params = std::nullopt;
        }

        RequestMessage(
            uint64_t id,
            std::string method,
            std::optional<T> params) :
        id(id), method(method), params(params) {
            this->jsonrpc = Message();
        }

    };
}

#endif //REQUESTMESSAGE_H
