
#pragma once
#include <iostream>
#include <vector>

//enum за следене на типа на токена
enum Element {
    CURLY_OPEN,
    CURLY_CLOSE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    COLON,
    COMMA,
    STRING,
    NUMBER,
    BOOLEAN,
    JSON_NULL,
    WHITESPACE,
    END_OF_FILE,
    UNKNOWN
};
//структурата на токена
struct Token {
    Element type;
    std::string value;
};
class Tokenizer {
    std::string fileContent;
    unsigned currIndex;
public:
    Tokenizer(const std::string& file) : fileContent(file), currIndex(0) {}
    //Помощни функции
    Token readString();
    Token readNumber();
    Token readBoolean();
    void skipWhitespace();
    Token getNextToken();
    //Връща масив от токени, които представят файла
    std::vector<Token> tokenize();

};