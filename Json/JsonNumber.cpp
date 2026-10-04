

#include "JsonNumber.h"

void JsonNumber::print(unsigned depth) const {
    std::cout << number;
}

std::string JsonNumber::getName() const {
    return "";

}

std::vector<std::string> JsonNumber::search(const std::string &key) const {
    std::vector<std::string> result;
    return result;
}

JsonValue * JsonNumber::clone() const {
    return new JsonNumber(*this);
}

void JsonNumber::set(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot access deeper into a number value");

}

void JsonNumber::createPath(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot create a path inside a number value");

}

void JsonNumber::deletePath(std::vector<std::string> path, unsigned depth) {
    throw std::runtime_error("Cannot delete a number value");

}

void JsonNumber::writeInFile(std::ostream &os) const {
    os << number;
}
std::string JsonNumber::toString() const {
    return std::to_string(number);
}
