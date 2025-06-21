//
// Created by grusted on 6/18/25.
//

#ifndef RANGE_H
#define RANGE_H
#include "../external/json.hpp"
#include "TextDocument/Rapams/CompletionParams.h"

namespace lsp_test {

    using json = nlohmann::json;

    struct Range {
        Position start;
        Position end;
    };

    void to_json(nlohmann::json& j, const Range& r) {
        j["end"] = r.end;
        j["start"] = r.start;
    }
}
#endif //RANGE_H
