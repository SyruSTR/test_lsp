//
// Created by grusted on 6/17/25.
//

#ifndef NOTIFICATIONMESSAGE_H
#define NOTIFICATIONMESSAGE_H
#include "Message.h"

namespace lsp_test {

    template <typename T>
    struct NotificationMessage : Message {
        std::string method;
        std::optional<T> params; //something else

        NotificationMessage() : Message() {}
        NotificationMessage(
            std::string method,
            std::optional<T> params) :
        method(method), params(params) {}
    };

    template <typename T>
    void from_json(const json& j, NotificationMessage<T> &msg) {
        msg = j.get<Message>();
        j.at("method").get_to(msg.method);
        if (j.contains("params") && !j["params"].is_null()) {
            msg.method = j["params"].get<T>();
        }
        else
            msg.method = std::nullopt;
    }
}

#endif //NOTIFICATIONMESSAGE_H
