//
// Created by grusted on 6/16/25.
//

#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H

#include "Message.h"
#include "TextDocument/CompletionParams.h"

namespace lsp_test {

    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    struct RequestMessage : Message {

        int64_t id;
        std::string method;
        std::optional<T> params;

        RequestMessage(
            const uint64_t id,
            const std::string &method,
            std::optional<T> params) :
        id(id), method(method), params(params) {}

        RequestMessage(
            uint64_t id,
            std::string method) :
        method(method), id(id) {
            params = std::nullopt;
        }

    };

    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    void from_json(const json &j, RequestMessage<T> &msg) {
        from_json(j,static_cast<Message &>(msg));

        j.at("id").get_to(msg.id);
        j.at("method").get_to(msg.method);
        if (j.contains("params") && !j["params"].is_null()) {
            msg.params = j.at("params").get<T>();
        }
        else
            msg.params = std::nullopt;
    }

    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    void to_json(json &j, const RequestMessage<T> &msg) {
        to_json(j,static_cast<const Message &>(msg));
        // j["id"] = msg.id;
        j.push_back({"id",msg.id});
        j.push_back({"method",msg.method});
        j.push_back({"params",msg.params});
    }
}

#endif //REQUESTMESSAGE_H
