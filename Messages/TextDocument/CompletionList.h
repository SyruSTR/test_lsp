//
// Created by grusted on 6/18/25.
//

#ifndef COMPLETIONLIST_H
#define COMPLETIONLIST_H
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

    void to_json(json& j, const CompletionList& cl) {
        j = {
            {"isIncomplete", cl.isIncomplete},
            {"items", cl.items}
        };
    }
}
#endif //COMPLETIONLIST_H
