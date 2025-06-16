//
// Created by grusted on 6/16/25.
//

#ifndef RESPONSEMESSAGE_H
#define RESPONSEMESSAGE_H

#include "Message.h"

struct ResponseMessage : Message {
    int64_t id;
    json result;

    ResponseMessage(const std::string &jsonrpc, int64_t id) : Message(jsonrpc), id(id) {}
    ResponseMessage(const int64_t id) : id(id) {
        this->jsonrpc = Message().jsonrpc;
    }
};

#endif //RESPONSEMESSAGE_H
