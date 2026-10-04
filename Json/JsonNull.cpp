#include "JsonNull.h"


void JsonNull::print(unsigned depth) const {
    std::cout << "null";
}

std::string JsonNull::getName() const {
      return "";
}

std::vector<std::string> JsonNull::search(const std::string &key) const {
    std::vector<std::string> result;
    return result;

}

JsonValue * JsonNull::clone() const {
    return new JsonNull(*this);
}

void JsonNull::set(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot access deeper into a null value");

}

void JsonNull::createPath(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    throw std::runtime_error("Cannot create a path inside a null value");

}

void JsonNull::deletePath(std::vector<std::string> path, unsigned depth) {
    throw std::runtime_error("Cannot delete a null value");
}

void JsonNull::writeInFile(std::ostream &os) const {
    os << "null";
}
std::string JsonNull::toString() const {
    return "null";
}
