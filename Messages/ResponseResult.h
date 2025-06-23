//
// Created by grusted on 6/18/25.
//

#ifndef RESPONCERESULT_H
#define RESPONCERESULT_H
#include "../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    struct ResponseResult {

    };

    void inline to_json(json& j, const ResponseResult& result) {}
}
#endif //RESPONCERESULT_H
