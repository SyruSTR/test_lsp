//
// Created by grusted on 6/16/25.
//

#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H

#include <optional>
#include "Message.h"
#include "NotificationMessage.h"

namespace lsp_test {
struct RequestMessage : NotificationMessage {

        int64_t id;

        RequestMessage(
            uint64_t id,
            std::string method,
            std::optional<DidChangeTextDocumentParams> params) :
        NotificationMessage(method,params), id(id) {}

        RequestMessage(
            uint64_t id,
            std::string method) :
        NotificationMessage(method, std::nullopt), id(id) {}

    };

    void from_json(const json &j, RequestMessage &msg) {
        from_json(j,static_cast<NotificationMessage &>(msg));
        j.at("id").get_to(msg.id);
    }
}

#endif //REQUESTMESSAGE_H
