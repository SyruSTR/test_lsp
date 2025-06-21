//
// Created by grusted on 6/20/25.
//

#ifndef DICTIONARYWORDS_H
#define DICTIONARYWORDS_H
#include <string>
#include <unordered_set>
#include "TextDocument/CompletionList.h"


class DictionaryWords {
public:
    DictionaryWords();
    ~DictionaryWords();

    bool Contains(const std::string& word) const;
    void AddWordsFromFile(std::string filePath);
    void WrapToCompletionList(lsp_test::CompletionList &completionList) const;
    void WrapToCompletionList(lsp_test::CompletionList &completionList, const std::string& starts_with) const;

private:

    std::unordered_set<std::string> dictionary;

};



#endif //DICTIONARYWORDS_H
