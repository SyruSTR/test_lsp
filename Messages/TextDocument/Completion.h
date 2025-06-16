//
// Created by grusted on 6/16/25.
//

#ifndef COMPLETION_H
#define COMPLETION_H
#include <string>
#include <vector>

#include "../ResponseMessage.h"
#include "../../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    struct CompletionItem {
        std::string label;

        CompletionItem(const std::string &label_) : label(label_) {}
    };

    void to_json(json &j, const CompletionItem &ci) {
        j = {{"label", ci.label}};
    }

    struct CompletionList {
        bool isIncomplete;
        std::vector<CompletionItem> items;

        CompletionList() : isIncomplete(false) {
            items.clear();
        }
    };

    json completion(const ResponseMessage& msg, CompletionList& ComList) {
        json response = msg;
        ComList.items.emplace_back("TypeScript");
        ComList.items.emplace_back("LSP");
        ComList.items.emplace_back("Lua");
        response["result"].push_back({"items",ComList.items});
        // for (const auto &item : ComList.items) {
        //     response["result"]["items"].push_back(item.label);
        // }
        return response;
    }
}


#endif //COMPLETION_H
