#include "SplitString.h"
#include <stdexcept>

//намира броя на думите в стринг според подаден разделител
int numberOfWords(const std::string& word, char divider) {
    if (word.empty()) {
        return 0;
    }
    int count = 1;
    for (size_t i = 0; i < word.length(); i++) {
        if (word[i] == divider) {
            count++;
        }
    }
    return count;
}

//намира дума в стринг според подаден разделител и място на думата
std::string getWord(const std::string& word, char divider, int wordPlace) {
    if (word.empty()) {
        return "";
    }

    int totalWords = numberOfWords(word, divider);
    if (wordPlace < 1 || wordPlace > totalWords) {
        throw std::invalid_argument("Invalid number of words");
    }

    unsigned  wordStart = 0;
    unsigned wordEnd = word.find(divider);
    int wordCount = 1;

    while (wordCount < wordPlace) {
        wordStart = wordEnd + 1;
        wordEnd = word.find(divider, wordStart);
        wordCount++;
    }

    std::string extracted;
    if (wordEnd == std::string::npos) {
        extracted = word.substr(wordStart);
    } else {
        extracted = word.substr(wordStart, wordEnd - wordStart);
    }

    return extracted;
}