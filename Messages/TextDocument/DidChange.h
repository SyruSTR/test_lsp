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

    json DidChange(const NotificationMessage &msg) {
        json j;
        to_json(j,msg);
        return j;
    }
}

#endif //DIDCHANGE_H
