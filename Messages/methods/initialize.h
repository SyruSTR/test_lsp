//
// Created by grusted on 6/16/25.
//

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "../RequestMessage.h"
#include "../ResponseMessage.h"
#include "../../external/json.hpp"
#include "../TextDocument/TextDocumentSync.h"

namespace lsp_test {

    using json = nlohmann::json;

    template<typename T, std::enable_if_t<std::is_base_of_v<ResponseResult, T>, bool> = true>
    json initialize_response(const ResponseMessage<T> &msg) {
        json response = msg;
        // response["result"]["capabilities"]["completionProvider"] = json::object();
        // response["result"]["capabilities"].push_back({"completionProvider",json::object()});
        // response["result"]["capabilities"]["textDocumentSync"] = FULL;
        // response["result"]["capabilities"].push_back({"textDocumentSync",FULL});
        return response;
    }
}

#endif //INITIALIZE_H
