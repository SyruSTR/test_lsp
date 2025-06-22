//
// Created by grusted on 6/22/25.
//

#ifndef TEXTDOCUMENTITEM_H
#define TEXTDOCUMENTITEM_H
#include "json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    struct TextDocumentItem {
        std::string uri;
        std::string languageId;
        int version;
        std::string text;
    };

    void inline from_json(const json& j, TextDocumentItem& textDocumentItem) {
        j.at("uri").get_to(textDocumentItem.uri);
        j.at("languageId").get_to(textDocumentItem.languageId);
        j.at("version").get_to(textDocumentItem.version);
        j.at("text").get_to(textDocumentItem.text);
    }
}
#endif //TEXTDOCUMENTITEM_H
