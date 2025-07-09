//
// Created by grusted on 6/20/25.
//

#ifndef DICTIONARYWORDS_H
#define DICTIONARYWORDS_H
#include <string>
#include "TextDocument/CompletionList.h"


class DictionaryWords {
public:
    DictionaryWords();
    // DictionaryWords(const std::string& filepath);
    ~DictionaryWords();

    bool Contains(const std::string& word) const;
    // void AddWordsFromFile(const std::string& filePath);
    // void WrapToCompletionList(lsp_test::CompletionList &completionList) const;
    void WrapToCompletionList(lsp_test::CompletionList &completionList, const std::string& starts_with) const;

private:
    const uint32_t MAX_LIST_LENGTH = 1000;
    void AddKeywords();
    // std::unordered_set<std::string> dictionary;
    std::map<std::string, lsp_test::CompletionItem> words;

};



#endif //DICTIONARYWORDS_H
