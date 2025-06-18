//
// Created by grusted on 6/17/25.
//

#ifndef COMPLITIONPARAMS_H
#define COMPLITIONPARAMS_H

#include "DidChangeTextParams.h"

namespace lsp_test {

    struct Position {
        int64_t line;
        int64_t character;
    };

    void from_json(const json& j, Position& pos) {
        j.at("line").get_to(pos.line);
        j.at("character").get_to(pos.character);
    }

    void to_json(json& j, const Position& pos) {
        j["line"] = pos.line;
        j["character"] = pos.character;
    }

    struct TextDocumentPositionParams : Params {
        TextDocumentIdentifier textDocument;
        Position position;
    };

    void from_json(const json& j, TextDocumentPositionParams& params) {
        j.at("textDocument").get_to(params.textDocument);
        j.at("position").get_to(params.position);
    }

    struct CompletionParams : TextDocumentPositionParams {

    };

}

#endif //COMPLITIONPARAMS_H
