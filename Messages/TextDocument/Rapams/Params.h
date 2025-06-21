//
// Created by grusted on 6/17/25.
//

#ifndef PARAMS_H
#define PARAMS_H

namespace lsp_test  {

    using json = nlohmann::json;

    struct Params {

    };

    void to_json(json& j, const Params& params) {}

};

#endif //PARAMS_H
