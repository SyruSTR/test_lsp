//
// Created by grusted on 6/18/25.
//

#ifndef FULLDOCUMENTDIAGNOSTICREPORT_H
#define FULLDOCUMENTDIAGNOSTICREPORT_H
#include "../../../external/json.hpp"
#include "../DiagnosticStruct.h"

namespace lsp_test {

    using json = nlohmann::json;

    struct FullDocumentDiagnosticReport {
        // for json named by 'kind'
        std::string documentDiagnosticReportKind = "full";
        // std::optional<std::string> resultId;
        std::vector<Diagnostic> items;

        FullDocumentDiagnosticReport() {
            items.clear();
        }
    };

    void to_json(nlohmann::json& j, const FullDocumentDiagnosticReport& r) {
        j["kind"] = r.documentDiagnosticReportKind;
        j["items"] = r.items;
    }

}
#endif //FULLDOCUMENTDIAGNOSTICREPORT_H
