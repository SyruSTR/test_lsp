//
// Created by grusted on 6/16/25.
//

#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H

#include <optional>
#include "Message.h"
#include "NotificationMessage.h"

namespace lsp_test {
    template <typename T>
struct RequestMessage : NotificationMessage<T> {

        int64_t id;

        RequestMessage(
            uint64_t id,
            std::string method,
            std::optional<T> params) :
        NotificationMessage<T>(method,params), id(id) {}

        RequestMessage(
            uint64_t id,
            std::string method) :
        NotificationMessage<T>(method, std::nullopt), id(id) {
            this->jsonrpc = Message();
        }

    };

    template <typename T>
    void from_json(const json &j, RequestMessage<T> &msg) {
        msg = j.get<NotificationMessage<T>>();
        j.at("id").get_to(msg.id);
    }
}

#endif //REQUESTMESSAGE_H
