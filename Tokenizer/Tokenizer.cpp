#include "Tokenizer.h"

Token Tokenizer::readString() {
    currIndex++;
    std::string str;
    while (fileContent[currIndex] != '\0' && fileContent[currIndex] != '"') {
        str.push_back(fileContent[currIndex]);
        currIndex++;
    }
    currIndex++;
    Token resultToken;
    resultToken.type = STRING;
    resultToken.value = str;
    return resultToken;
}

Token Tokenizer::readNumber() {
    std::string str;
    if (fileContent[currIndex] == '-') {
        str.push_back('-');
        currIndex++;
    }
    while (fileContent[currIndex] != '\0' && (std::isdigit(fileContent[currIndex]) || fileContent[currIndex] == '.')) {
        str.push_back(fileContent[currIndex]);
        currIndex++;
    }
    Token resultToken;
    resultToken.type = NUMBER;
    resultToken.value = str;
    return resultToken;

}

Token Tokenizer::readBoolean() {
    Token resultToken;
    resultToken.type = BOOLEAN;

    if (currIndex + 4 <= fileContent.size() && fileContent.substr(currIndex, 4) == "true") {
        resultToken.value = "true";
        currIndex += 4;
        return resultToken;
    }
    else if (currIndex + 5 <= fileContent.size() && fileContent.substr(currIndex, 5) == "false") {
        resultToken.value = "false";
        currIndex += 5;
        return resultToken;
    }
    throw std::invalid_argument("Invalid boolean value");
}

void Tokenizer::skipWhitespace() {
    while (fileContent[currIndex] != '\0' && (fileContent[currIndex] == ' '|| fileContent[currIndex] == '\t' || fileContent[currIndex] == '\n' || fileContent[currIndex] == '\r')) {
        currIndex++;
    }
}

Token Tokenizer::getNextToken() {
    skipWhitespace();
    char curr = fileContent[currIndex];
    Token resultToken;
    if (curr == '\0') {
        resultToken.type = END_OF_FILE;
        resultToken.value = "";
        return resultToken;
    }

    switch (curr) {
        case '{':
            currIndex++;
            resultToken.value = "{";
            resultToken.type = CURLY_OPEN;
            return resultToken;
        case '}':
            currIndex++;
            resultToken.value = "}";
            resultToken.type = CURLY_CLOSE;
            return resultToken;

        case '[':
            currIndex++;
            resultToken.value = "[";
            resultToken.type = BRACKET_OPEN;
            return resultToken;

        case ']':
            currIndex++;
            resultToken.value = "]";
            resultToken.type = BRACKET_CLOSE;
            return resultToken;

        case ':':
            currIndex++;
            resultToken.value = ":";
            resultToken.type = COLON;
            return resultToken;

        case ',':
            currIndex++;
            resultToken.value = ",";
            resultToken.type = COMMA;
            return resultToken;
        case '"':
            return readString();
    }


    if (std::isdigit(curr) || curr == '-') {
        return readNumber();
    }
    if (curr == 'n') {
        if (fileContent.substr(currIndex, 4) == "null") {
            currIndex += 4;
            resultToken.type = JSON_NULL;
            resultToken.value = "null";
            return resultToken;
        }
    }
    if (curr == 't' || curr == 'f') {
        Token isBoolean = readBoolean();
        if (isBoolean.value == "true" || isBoolean.value == "false") {
            return isBoolean;
        }
        throw std::invalid_argument("File is not Json compliant");
    }
    resultToken.type = UNKNOWN;
    return resultToken;

}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> result;
    currIndex = 0;

    while (true) {
        Token t = getNextToken();

        if (t.type == UNKNOWN) {
            throw std::invalid_argument("Unknown symbol: " + t.value);
        }

        if (t.type == END_OF_FILE) {
            break;
        }

        result.push_back(t);
    }
    return result;

}
