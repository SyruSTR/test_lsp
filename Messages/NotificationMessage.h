//
// Created by grusted on 6/17/25.
//

#ifndef NOTIFICATIONMESSAGE_H
#define NOTIFICATIONMESSAGE_H
#include <variant>

#include "Message.h"
#include "TextDocument/DidChangeTextParams.h"
#include "TextDocument/Params.h"


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

    // void from_json(const json& j, std::variant<std::vector<std::string>, std::string>& v) {
    //     if (j.is_array()) {
    //         v = j.get<std::vector<std::string>>();
    //     } else if (j.is_string()) {
    //         v = j.get<std::string>();
    //     } else {
    //         throw std::invalid_argument("Expected string or array of strings for 'params'");
    //     }
    // }

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
            // std::variant<std::vector<std::string>, std::string> tmp;
            // from_json(j.at("params"), tmp);
            // msg.params = std::move(tmp);
            msg.params = j.at("params").get<T>();
        }
        else
            msg.params = std::nullopt;
    }
}

#endif //NOTIFICATIONMESSAGE_H
