//
// Created by grusted on 6/18/25.
//

#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
#include "FullDocumentDiagnosticReport.h"

namespace lsp_test {

    template <typename T, std::enable_if_t<std::is_base_of_v<Params, T>, bool> = true>
    json diagnostic(RequestMessage<T> & msg) {
        json j;
        return j;
    }

}
#endif //DIAGNOSTIC_H
