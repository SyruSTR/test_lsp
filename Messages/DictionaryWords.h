//
// Created by grusted on 6/20/25.
//

#ifndef DICTIONARYWORDS_H
#define DICTIONARYWORDS_H
#include <string>
#include <unordered_set>
#include <vector>


class DictionaryWords {
public:
    DictionaryWords();
    ~DictionaryWords();

    bool Contains(const std::string& word) const;

private:

    std::unordered_set<std::string> dictionaty;

    void AddWordsFromFile(std::string filePath);

};



#endif //DICTIONARYWORDS_H
