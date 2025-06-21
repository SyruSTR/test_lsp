//
// Created by grusted on 6/21/25.
//

#ifndef TEXTDOCUMENT_H
#define TEXTDOCUMENT_H
#include <string>

#include "Rapams/DidChangeTextParams.h"
#include "../../Logger/Logger.h"
#include "../DictionaryWords.h"
#include "Result/CompletionResult.h"


class DictionaryWords;

namespace lsp_test {

    class TextDocument {
    public:
        TextDocument(std::string *currentLine);
        ~TextDocument();

        void parseMessage(const std::string& raw_message);

    private:
        std::unique_ptr<DictionaryWords> m_dictionary;
        std::map<TextDocumentIdentifier, std::string> m_textDocuments;
        std::unique_ptr<Logger> m_logger;
        std::shared_ptr<std::string> m_currentReadedLine;

        std::string m_method;

        void completion(const nlohmann::json& j );

        void diagnostic(const nlohmann::json& j);

        void didChange(const nlohmann::json& j);

        void initialize_response(const nlohmann::json& j);

        void sendResponse(const nlohmann::json& response);
    };
}




#endif //TEXTDOCUMENT_H
