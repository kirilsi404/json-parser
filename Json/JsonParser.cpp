

#include "JsonParser.h"

JsonValue * JsonParser::parseValue() {
    Token curr = tokenizedFile[index];
    switch (curr.type) {
        case CURLY_OPEN:
            return parseObject();
        case BRACKET_OPEN:
            return parseArray();
        case STRING:
            return parseString();
        case NUMBER:
            return parseNumber();
        case BOOLEAN:
            return parseBoolean();
        case JSON_NULL:
            return parseNull();
        default:
            throw std::runtime_error("Invalid token");
}
}

JsonValue * JsonParser::parseObject() {
    index++; // Прескачаме '{'
    JsonObject* newObject = new JsonObject();

    while (index < tokenizedFile.size() && tokenizedFile[index].type != CURLY_CLOSE) {
        if (tokenizedFile[index].type != STRING) {
            delete newObject;
            throw std::runtime_error("Expected string key inside object");
        }
        std::string name = tokenizedFile[index].value;
        index++;

        if (index >= tokenizedFile.size() || tokenizedFile[index].type != COLON) {
            delete newObject;
            throw std::runtime_error("Expected colon inside object");
        }
        index++;

        JsonValue* value = parseValue();
        newObject->addPair(name, value);

        if (index >= tokenizedFile.size()) {
            delete newObject;
            throw std::runtime_error("Missing closing curly bracket");
        }

        if (tokenizedFile[index].type == COMMA) {
            index++;
            if (index < tokenizedFile.size() && tokenizedFile[index].type == CURLY_CLOSE) {
                delete newObject;
                throw std::runtime_error("Trailing comma inside object");
            }
        } else if (tokenizedFile[index].type != CURLY_CLOSE) {
            delete newObject;
            throw std::runtime_error("Expected comma or closing curly bracket");
        }
    }

    if (index >= tokenizedFile.size()) {
        delete newObject;
        throw std::runtime_error("Unclosed JSON object");
    }

    index++;
    return newObject;

}

JsonValue * JsonParser::parseArray() {
    index++;
    JsonArray* newArray = new JsonArray();
    while (tokenizedFile[index].type != BRACKET_CLOSE) {

        JsonValue* value = parseValue();
        newArray->addElement(value);

        if (tokenizedFile[index].type == COMMA) {
            index++;
        } else if (tokenizedFile[index].type != BRACKET_CLOSE) {
            throw std::runtime_error("Expected comma or closing  bracket");
        }
    }
    index++;
    return newArray;
}

JsonValue * JsonParser::parseString() {
    std::string str = tokenizedFile[index].value;
    index++;
    return new JsonString(str);
}

JsonValue * JsonParser::parseNumber() {
    std::string val = tokenizedFile[index].value;
    index++;

    double result = 0.0;
    bool isNegative = false;
    size_t i = 0;

    if (val[i] == '-') {
        isNegative = true;
        i++;
    }
    while (i < val.size() && val[i] != '.') {
        result = result * 10.0 + (val[i] - '0');
        i++;
    }

    if (i < val.size() && val[i] == '.') {
        i++;
        double weight = 0.1;

        while (i < val.size()) {
            result += (val[i] - '0') * weight;
            weight /= 10.0;
            i++;
        }
    }
    if (isNegative) {
        result = -result;
    }
    return new JsonNumber(result);
}


JsonValue * JsonParser::parseBoolean() {
    std::string str = tokenizedFile[index].value;
    index++;
    bool val =(str == "true");
    return new JsonBool(val);
}

JsonValue * JsonParser::parseNull() {
    index++;
    return new JsonNull();
}

bool JsonParser::validate(std::istream &is) {
    std::string fileContent;
    std::string line;
    while (std::getline(is, line)) {
        fileContent += line + "\n";
    }
       try {
        Tokenizer tokenizer(fileContent);
        std::vector<Token> tokens = tokenizer.tokenize();

        JsonParser parser(tokens);
        JsonValue* temporaryTree = parser.parse();

        delete temporaryTree;
        return true;

    } catch (const std::exception& e) {

        return false;
    }
}

JsonValue* JsonParser::parse() {
    if (tokenizedFile.empty()) {
        throw std::runtime_error("Empty token list");
    }
    Element firstType = tokenizedFile[0].type;
    if (firstType != CURLY_OPEN && firstType != BRACKET_OPEN) {
        throw std::runtime_error("A valid json document must start with an object or an array");
    }
    JsonValue* root = parseValue();
    return root;

}

JsonParser::~JsonParser() {
}
