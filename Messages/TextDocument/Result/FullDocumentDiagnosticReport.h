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
            // Diagnostic diagnostic1;
            // diagnostic1.severity = ERROR;
            // diagnostic1.source = "My LSP!!!";
            // diagnostic1.message = "This is incorrect";
            // diagnostic1.range = {{0,4},{0,8}};
            // items.push_back(diagnostic1);
            //
            // Diagnostic diagnostic2;
            // diagnostic2.severity = WARNING;
            // diagnostic2.source = "My LSP!!!";
            // diagnostic2.message = "This could be better";
            // diagnostic2.range = {{1,6},{1,8}};
            // items.push_back(diagnostic2);
        }
    };

    void to_json(nlohmann::json& j, const FullDocumentDiagnosticReport& r) {
        j["kind"] = r.documentDiagnosticReportKind;
        j["items"] = r.items;
    }

}
#endif //FULLDOCUMENTDIAGNOSTICREPORT_H
