//
// Created by grusted on 6/18/25.
//

#ifndef COMPLETIONLIST_H
#define COMPLETIONLIST_H
#include "../../external/json.hpp"

namespace lsp_test {

#define MAX_LENGTH 1000

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

    void to_json(json& j, const CompletionList& cl) {
        j = {
            {"isIncomplete", cl.items.capacity() == MAX_LENGTH},
            {"items", cl.items}
        };
    }
}
#endif //COMPLETIONLIST_H
