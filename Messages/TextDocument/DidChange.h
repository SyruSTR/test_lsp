//
// Created by grusted on 6/17/25.
//

#ifndef DIDCHANGE_H
#define DIDCHANGE_H
#include <string>
#include <vector>
#include "../NotificationMessage.h"

namespace lsp_test {

    using json = nlohmann::json;

    template<typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    json DidChange(const NotificationMessage<T> &msg) {
        json j;
        to_json(j,msg);
        return j;
    }
}

#endif //DIDCHANGE_H
