//
// Created by grusted on 6/18/25.
//

#ifndef RANGE_H
#define RANGE_H
#include "../external/json.hpp"
#include "TextDocument/Params/CompletionParams.h"

namespace lsp_test {

    using json = nlohmann::json;

    struct Range {
        Position start;
        Position end;

        Range(const Position& start, const Position& end): start(start), end(end) {}
        Range() { start = Position(0, 0); end = Position(0, 0); }
    };

    void to_json(nlohmann::json& j, const Range& r) {
        j["end"] = r.end;
        j["start"] = r.start;
    }
}
#endif //RANGE_H
