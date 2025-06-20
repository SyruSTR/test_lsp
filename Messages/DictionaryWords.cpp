//
// Created by grusted on 6/20/25.
//

#include "DictionaryWords.h"

#include <fstream>

DictionaryWords::DictionaryWords() {
    dictionaty.clear();
}

DictionaryWords::~DictionaryWords() = default;

void DictionaryWords::AddWordsFromFile(std::string filePath) {
    if (std::ifstream wordsFile(filePath); wordsFile.is_open()) {
        std::string bufferLine;
        while (wordsFile.good() && std::getline(wordsFile, bufferLine)) {
            dictionaty.insert(bufferLine);
        }
    }
}

bool DictionaryWords::Contains(const std::string& word) const {
    return dictionaty.contains(word);
}


