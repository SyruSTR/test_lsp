//
// Created by grusted on 6/17/25.
//

#ifndef DIDCHANGE_H
#define DIDCHANGE_H
#include <string>
#include <vector>

namespace lsp_test {

    struct TextDocumentIdentifier {
        std::string documentUri;
    };

    struct VersionedTextDocumentIdentifier : TextDocumentIdentifier {
        int64_t version;
    };

    struct TextDocumentContentChangeEvent {
        std::string text;
    };

    struct DidChangeTextDocumentParams {
        VersionedTextDocumentIdentifier textDocument;
        std::vector<TextDocumentContentChangeEvent> contentChange;
    };

    json DidChange() {

    }
}

#endif //DIDCHANGE_H
