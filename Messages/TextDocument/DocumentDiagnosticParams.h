//
// Created by grusted on 6/18/25.
//

#ifndef DOCUMENTDIAGNOSTICPARAMS_H
#define DOCUMENTDIAGNOSTICPARAMS_H
#include "DidChangeTextParams.h"
#include "Params.h"
#include "../../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    struct DocumentDiagnosticParams : Params {
        TextDocumentIdentifier textDocument;
    };

    void from_json(const json &j, DocumentDiagnosticParams &params) {
        j.at("textDocument").get_to(params.textDocument);
    }
}

#endif //DOCUMENTDIAGNOSTICPARAMS_H
