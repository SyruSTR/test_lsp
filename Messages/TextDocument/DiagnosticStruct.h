//
// Created by grusted on 6/18/25.
//

#ifndef DIAGNOSTICSTRUCT_H
#define DIAGNOSTICSTRUCT_H
#include <string>

#include "Rapams/CompletionParams.h"
#include "../Range.h"

namespace lsp_test {

    using json = nlohmann::json;

    enum DiagnosticSeverity {
        ERROR = 1,
        WARNING = 2,
        INFORMATION = 3,
        HINT = 4,
    };

    struct Diagnostic {
        Range range;
        DiagnosticSeverity severity;
        std::string source = "My tested LSP";
        std::string message;
        // data

        Diagnostic(): range(Position()), severity(ERROR) {
        }

        bool operator<(const Diagnostic &other) const {
            //TODO replace to another
            return message < other.message;
        }
    };

    void to_json(nlohmann::json &j, const Diagnostic &d) {
        j["range"] = d.range;
        j["severity"] = d.severity;
        j["source"] = d.source;
        j["message"] = d.message;
    }
}
#endif //DIAGNOSTICSTRUCT_H
