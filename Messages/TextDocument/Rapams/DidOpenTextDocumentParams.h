//
// Created by grusted on 6/22/25.
//

#ifndef DIDOPENTEXTDOCUMENTPARAMS_H
#define DIDOPENTEXTDOCUMENTPARAMS_H
#include "json.hpp"
#include "Params.h"
#include "../TextDocumentItem.h"

namespace lsp_test {
    using json = nlohmann::json;

    struct DidOpenTextDocumentParams : Params {
        TextDocumentItem textDocument;
    };

    inline void from_json(const json &j, DidOpenTextDocumentParams& params) {
        j.at("textDocument").get_to(params.textDocument);
    }
}
#endif //DIDOPENTEXTDOCUMENTPARAMS_H
