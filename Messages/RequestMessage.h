//
// Created by grusted on 6/16/25.
//

#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H

#include "Message.h"

template <typename T>
struct RequestMessage:Message {
    int64_t id;
    std::string method;
    T params; //something else


};

#endif //REQUESTMESSAGE_H
