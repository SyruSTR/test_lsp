//
// Created by grusted on 6/16/25.
//

#ifndef COMPLETION_H
#define COMPLETION_H
#include <string>
#include <vector>

#include "../../external/json.hpp"

struct CompletionItem {
    std::string label;

    CompletionItem(const std::string &label_) : label(label_) {}
};

struct CompletionList {
    bool isIncomplete;
    std::vector<CompletionItem> items;

    nlohmann::json test_CompletionList() {
        nlohmann::json result;
        result["isIncomplete"] = false;

        items.emplace_back("TypeScript");
        items.emplace_back("LSP");
        items.emplace_back("Lua");
        for (const auto &item : items) {
            result["items"].push_back(item.label);
        }
        // result["items"] = nlohmann::json::parse(items.begin(),items.end());

        return result;
    }
};

#endif //COMPLETION_H
