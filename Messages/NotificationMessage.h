//
// Created by grusted on 6/17/25.
//

#ifndef NOTIFICATIONMESSAGE_H
#define NOTIFICATIONMESSAGE_H
#include <variant>

#include "Message.h"
#include "TextDocument/Rapams/DidChangeTextParams.h"
#include "TextDocument/Rapams/Params.h"


namespace lsp_test {



    using json = nlohmann::json;
    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    struct NotificationMessage : Message {
        std::string method;
        std::optional<T> params; //something else

        NotificationMessage() {}
        NotificationMessage(
            const std::string &method,
            const std::optional<T> &params) :
        method(method), params(params) {}
    };

    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    void to_json(json &j, const NotificationMessage<T> &msg) {
        to_json(j,static_cast<Message>(msg));
        j["method"] = msg.method;
        j["params"] = msg.params;
    }

    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    void from_json(const json& j, NotificationMessage<T> &msg) {
        from_json(j, static_cast<Message&>(msg));

        j.at("method").get_to(msg.method);
        if (j.contains("params") && !j["params"].is_null()) {
            msg.params = j.at("params").get<T>();
        }
        else
            msg.params = std::nullopt;
    }
}

#endif //NOTIFICATIONMESSAGE_H
