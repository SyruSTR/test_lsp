//
// Created by grusted on 6/18/25.
//

#ifndef COMPLETIONRESULT_H
#define COMPLETIONRESULT_H
#include "../CompletionList.h"
#include "../../ResponseResult.h"
#include "../../../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    struct CompletionResult : ResponseResult {
        CompletionList completion_list;

        CompletionResult() {
            completion_list = CompletionList();
        }
    };

    void inline to_json(json &j, const CompletionResult &r) {
        to_json(j, r.completion_list);
    }
}

#endif //COMPLETIONRESULT_H
