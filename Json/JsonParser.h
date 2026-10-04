#pragma once
#include "JsonValue.h"
#include "../Tokenizer/Tokenizer.h"
#include "JsonObject.h"
#include "JsonArray.h"
#include "JsonBool.h"
#include "JsonNull.h"
#include "JsonNumber.h"
#include "JsonString.h"
class JsonParser {
    std::vector<Token> tokenizedFile;
    unsigned index;
    //помощни функции
    JsonValue* parseObject();
    JsonValue* parseArray();
    JsonValue* parseString();
    JsonValue* parseNumber();
    JsonValue* parseBoolean();
    JsonValue* parseNull();
public:
    JsonParser(std::vector<Token> file) : tokenizedFile(file),index(0) {}
    //определя коя от помощните фунции да извика
    JsonValue* parseValue();
    //проверява дали е валиден файла
    bool validate(std::istream& is);
    //създава дървото
    JsonValue* parse();
    ~JsonParser();

};