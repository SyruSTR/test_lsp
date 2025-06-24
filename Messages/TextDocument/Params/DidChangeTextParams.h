//
// Created by grusted on 6/17/25.
//

#ifndef DIDCHANGETEXTPARAMS_H
#define DIDCHANGETEXTPARAMS_H
#include "Params.h"
#include "../../../external/json.hpp"

namespace lsp_test {

    using json = nlohmann::json;

    struct TextDocumentIdentifier {
        std::string uri;

        // needed for std::map
        bool operator<(const TextDocumentIdentifier& id) const {
            return uri < id.uri;
        }
    };



    void inline  to_json(json &j, const TextDocumentIdentifier &id) {
        j["uri"] = id.uri;
    }

    void inline  from_json(const nlohmann::json& j, TextDocumentIdentifier& id) {
        j.at("uri").get_to(id.uri);
    }

    struct VersionedTextDocumentIdentifier : TextDocumentIdentifier {
        int64_t version;
    };

    void inline  to_json(json &j, const VersionedTextDocumentIdentifier &id) {
        to_json(j, static_cast<TextDocumentIdentifier>(id));
        j["version"] = id.version;
    }

    void inline from_json(const json& j, VersionedTextDocumentIdentifier& id) {
        from_json(j, static_cast<TextDocumentIdentifier &>(id));
        j.at("version").get_to(id.version);
    }

    struct TextDocumentContentChangeEvent {
        std::string text;
    };

    void inline  to_json(json &j, const TextDocumentContentChangeEvent &event) {
        j["text"] = event.text;
    }

    void inline from_json(const json& j, TextDocumentContentChangeEvent& event) {
        j.at("text").get_to(event.text);
    }

    struct DidChangeTextDocumentParams : Params {
        VersionedTextDocumentIdentifier textDocument;
        std::vector<TextDocumentContentChangeEvent> contentChange;
    };

    void inline to_json(json &j, const DidChangeTextDocumentParams &params) {
        j["textDocument"] = params.textDocument;
        j["contentChanges"] = params.contentChange;
    }

    void inline from_json(const json& j, DidChangeTextDocumentParams &params) {
        j.at("textDocument").get_to(params.textDocument);
        j.at("contentChanges").get_to(params.contentChange);
    }

}
#endif //DIDCHANGETEXTPARAMS_H
