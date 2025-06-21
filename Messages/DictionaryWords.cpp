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
    // for (auto &word : dictionary) {
    //     completionList.items.emplace_back(word);
    // }
    std::vector<std::string> buffer;
    std::ranges::copy_if(dictionary,std::back_inserter(buffer),
                         [starts_with](const std::string& word) {
                             return word.starts_with(starts_with);
                         });

    for (auto &word : buffer) {
        completionList.items.emplace_back(word);
    }
}

bool DictionaryWords::Contains(const std::string& word) const {
    return dictionary.contains(word);
}


