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
        return response;
    }
}

#endif //INITIALIZE_H
