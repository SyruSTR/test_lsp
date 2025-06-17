//
// Created by grusted on 6/17/25.
//

#ifndef NOTIFICATIONMESSAGE_H
#define NOTIFICATIONMESSAGE_H
#include <variant>

#include "Message.h"
#include "TextDocument/DidChangeTextParams.h"

namespace lsp_test {

    using json = nlohmann::json;

    struct NotificationMessage : Message {
        std::string method;
        std::optional<DidChangeTextDocumentParams> params; //something else

        NotificationMessage() {}
        NotificationMessage(
            const std::string &method,
            const std::optional<DidChangeTextDocumentParams> &params) :
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

    void to_json(json &j, const NotificationMessage &msg) {
        to_json(j,static_cast<Message>(msg));
        j.push_back({"method",msg.method});
        // j["method"] = msg.method;
        j.push_back({"params",msg.params});
    }

    void from_json(const json& j, NotificationMessage &msg) {
        from_json(j, static_cast<Message&>(msg));

        j.at("method").get_to(msg.method);
        if (j.contains("params") && !j["params"].is_null()) {
            // std::variant<std::vector<std::string>, std::string> tmp;
            // from_json(j.at("params"), tmp);
            // msg.params = std::move(tmp);
            msg.params = j.at("params").get<DidChangeTextDocumentParams>();
        }
        else
            msg.params = std::nullopt;
    }
}

#endif //NOTIFICATIONMESSAGE_H
