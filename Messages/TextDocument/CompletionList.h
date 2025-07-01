//
// Created by grusted on 6/18/25.
//

#ifndef COMPLETIONLIST_H
#define COMPLETIONLIST_H
#include "../../external/json.hpp"

namespace lsp_test {

#define MAX_LENGTH 1000

    using json = nlohmann::json;

    enum CompletionItemKind {
        Text = 1,
        Method = 2,
        Function = 3,
        Constructor = 4,
        Field = 5,
        Variable = 6,
        Class = 7,
        Interface = 8,
        Module = 9,
        Property = 10,
        Unit = 11,
        Value = 12,
        Enum = 13,
        Keyword = 14,
        Snippet = 15,
        Color = 16,
        File = 17,
        Reference = 18,
        Folder = 19,
        EnumMember = 20,
        Constant = 21,
        Struct = 22,
        Event = 23,
        Operator = 24,
        TypeParameter = 25
    };

    struct CompletionItem {
        std::string label;
        CompletionItemKind kind;

        CompletionItem(const std::string &label_) : label(label_),kind(Text) {}
        CompletionItem(const std::string &label_, const CompletionItemKind kind_) : label(label_), kind(kind_) {}
    };

    inline void to_json(json &j, const CompletionItem &ci) {
        j = {{"label", ci.label}};
    }

    struct CompletionList {
        bool isIncomplete;
        std::vector<CompletionItem> items;

        CompletionList() : isIncomplete(false) {
            items.clear();
        }
    };

    inline void to_json(json& j, const CompletionList& cl) {
        j = {
            {"isIncomplete", cl.items.capacity() == MAX_LENGTH},
            {"items", cl.items}
        };
    }
}
#endif //COMPLETIONLIST_H
