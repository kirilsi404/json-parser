

#include "JsonString.h"


void JsonString::print(unsigned depth) const {
    std::cout << '"' << str << '"';
}

std::string JsonString::getName() const {
    return "";
}

std::vector<std::string> JsonString::search(const std::string &key) const {
    std::vector<std::string> result;
    return result;
}

JsonValue* JsonString::clone() const {
    return new JsonString(*this);
}

void JsonString::set(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot access deeper into a string value");

}

void JsonString::createPath(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot access deeper into a string value");

}

void JsonString::deletePath(std::vector<std::string> path, unsigned depth) {
    throw std::runtime_error("Cannot access deeper into a string value");

}

void JsonString::writeInFile(std::ostream &os) const {
    os << '"' << str << '"';
}
std::string JsonString::toString() const {
    return "\"" + str + "\"";
}
