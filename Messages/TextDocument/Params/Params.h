//
// Created by grusted on 6/17/25.
//

#ifndef PARAMS_H
#define PARAMS_H

#include "../external/json.hpp"

namespace lsp_test  {

    using json = nlohmann::json;

    struct Params {

    };

    void inline to_json(json& j, const Params& params) {}

}

#endif //PARAMS_H
