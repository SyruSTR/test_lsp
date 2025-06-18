//
// Created by grusted on 6/16/25.
//

#ifndef RESPONSEMESSAGE_H
#define RESPONSEMESSAGE_H

#include "Message.h"
#include "methods/InitializeResult.h"
#include "methods/ServerInfo.h"

namespace lsp_test {

    using json = nlohmann::json;

    template<typename T, std::enable_if_t<std::is_base_of_v<ResponseResult, T>, bool> = true>
    struct ResponseMessage : Message {
        int64_t id;
        T result;

        ResponseMessage(const std::string &jsonrpc, const int64_t id, const T result) :
        Message(jsonrpc),
        id(id),
        result(result){}
            // ServerInfo{},
            // [] {
            //     //TODO json -> ServerCapabilities
            //      json serverCapabilities;
            //      serverCapabilities["capabilities"]["completionProvider"] = json::object();
            //      serverCapabilities["capabilities"]["textDocumentSync"] = FULL;
            //      return serverCapabilities;
            // }()){}

        ResponseMessage(const int64_t id, const T result) :
        id(id),
        result(result)
        //     ServerInfo{},
        //     [] {
        //         //TODO json -> ServerCapabilities
        //         json serverCapabilities;
        //         serverCapabilities["capabilities"]["completionProvider"] = json::object();
        //         serverCapabilities["capabilities"]["textDocumentSync"] = FULL;
        //         return serverCapabilities;
        //     }()
        // )
        {
            this->jsonrpc = Message().jsonrpc;
        }
    };

    template<typename T, std::enable_if_t<std::is_base_of_v<ResponseResult, T>, bool> = true>
    void to_json(json &j, const ResponseMessage<T> &msg) {
        j = json{
                {"jsonrpc", msg.jsonrpc},
                {"id", msg.id},
                {"result", msg.result}
        };
    }

}


#endif //RESPONSEMESSAGE_H
