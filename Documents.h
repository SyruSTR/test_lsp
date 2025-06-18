//
// Created by grusted on 6/17/25.
//

#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <map>
#include <string>


namespace lsp_test {

    struct  Documents {
        //uri -> textDocumentBody
        std::map<TextDocumentIdentifier, std::string> textDocuments;

        Documents() {
            textDocuments.clear();
        }
    };

}

#endif //DOCUMENT_H
