//
// Created by grusted on 6/20/25.
//

#include "DictionaryWords.h"

#include <fstream>

DictionaryWords::DictionaryWords() {
    dictionary.clear();
}

DictionaryWords::DictionaryWords(const std::string &filepath) {
    dictionary.clear();
    AddWordsFromFile(filepath);
}


DictionaryWords::~DictionaryWords() = default;

void DictionaryWords::AddKeywords() {
    words.insert({
        {"double", lsp_test::CompletionItem("double", lsp_test::Keyword)},
        {"int", lsp_test::CompletionItem("int", lsp_test::Keyword)},
        {"string", lsp_test::CompletionItem("string", lsp_test::Keyword)},
        {"var", lsp_test::CompletionItem("var", lsp_test::Keyword)},
        {"if", lsp_test::CompletionItem("if", lsp_test::Keyword)},
        {"else", lsp_test::CompletionItem("else", lsp_test::Keyword)},
        {"func", lsp_test::CompletionItem("func", lsp_test::Keyword)},
        {"return", lsp_test::CompletionItem("return", lsp_test::Keyword)},
        {"let", lsp_test::CompletionItem("let", lsp_test::Keyword)},
        {"nil", lsp_test::CompletionItem("nil", lsp_test::Keyword)},
        {"while", lsp_test::CompletionItem("while", lsp_test::Keyword)}
    });
}


void DictionaryWords::AddWordsFromFile(const std::string& filePath) {
    if (std::ifstream wordsFile(filePath); wordsFile.is_open()) {
        std::string bufferLine;
        while (wordsFile.good() && std::getline(wordsFile, bufferLine)) {
            dictionary.insert(bufferLine);
        }
    }
}

void DictionaryWords::WrapToCompletionList(lsp_test::CompletionList &completionList) const {
    for (auto &word : dictionary) {
        completionList.items.emplace_back(word);
    }
}


void DictionaryWords::WrapToCompletionList(lsp_test::CompletionList &completionList, const std::string& starts_with) const {

    std::vector<std::string> buffer;
    std::ranges::copy_if(dictionary,std::back_inserter(buffer),
                         [starts_with](const std::string& word) {
                             return word.starts_with(starts_with);
                         });

    // cutting the buffer to 1000 elements
    const auto it_end = buffer.size() > MAX_LENGTH ? buffer.begin()+MAX_LENGTH : buffer.end();
    std::copy(buffer.begin(), it_end, std::back_inserter(completionList.items));
}

bool DictionaryWords::Contains(const std::string& word) const {
    return dictionary.contains(word);
}


