

#include "JsonBool.h"

void JsonBool::print(unsigned depth) const {
    std::string outResult = value ? "true" : "false";
    std::cout << outResult;
}

std::string JsonBool::getName() const {
    return "";
}

std::vector<std::string> JsonBool::search(const std::string &key) const {
    std::vector<std::string> result;
    return result;
}

JsonValue * JsonBool::clone() const {
    return new JsonBool(*this);
}

void JsonBool::set(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot access deeper into a bool value");
}

void JsonBool::createPath(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot create a path inside a bool value");
}

void JsonBool::deletePath(std::vector<std::string> path, unsigned depth) {
    throw std::runtime_error("Cannot delete inside a bool value");
}

void JsonBool::writeInFile(std::ostream &os) const {
    os << (value ? "true" : "false");
}
std::string JsonBool::toString() const {
    return value ? "true" : "false";
}
