//
// Created by grusted on 6/18/25.
//

#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
#include "FullDocumentDiagnosticReport.h"

namespace lsp_test {

    // template <typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    FullDocumentDiagnosticReport diagnostic(){
    // FullDocumentDiagnosticReport diagnostic(RequestMessage<T> & msg) {
        const auto report = FullDocumentDiagnosticReport();
        return report;
    }

}
#endif //DIAGNOSTIC_H
