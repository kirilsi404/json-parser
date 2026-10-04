
#include "JsonArray.h"
#include "JsonObject.h"
#include "../SplitString/SplitString.h"
#include <iostream>
#include <stdexcept>
#include <cctype>

static bool parseIndex(const std::string& key, size_t& index) {
    if (key.empty()) return false;
    for (char c : key) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    try {
        index = std::stoul(key);
        return true;
    } catch (...) {
        return false;
    }
}

void JsonArray::print(unsigned depth) const {
    std::cout << '[';
    for (size_t i = 0; i < array.size(); i++) {
        if (array[i] != nullptr) {
            array[i]->print(depth);
        }
        if (i + 1 < array.size()) {
            std::cout << ", ";
        }
    }
    std::cout << ']';
}

void JsonArray::addElement(JsonValue* element) {
    array.push_back(element);
}

JsonValue* JsonArray::findPath(const std::vector<std::string>& path, unsigned depth) {
    if (depth == path.size()) {
        return this;
    }

    size_t index = 0;
    if (!parseIndex(path[depth], index)) {
        return nullptr;
    }

    if (index < array.size() && array[index] != nullptr) {
        return array[index]->findPath(path, depth + 1);
    }

    return nullptr;
}

std::string JsonArray::getName() const {
    return "";
}

std::vector<std::string> JsonArray::search(const std::string& key) const {
    std::vector<std::string> result;
    for (size_t i = 0; i < array.size(); i++) {
        if (array[i] != nullptr) {
            std::vector<std::string> innerResult = array[i]->search(key);
            for (size_t j = 0; j < innerResult.size(); j++) {
                result.push_back(innerResult[j]);
            }
        }
    }
    return result;
}

JsonValue* JsonArray::clone() const {
    JsonArray* copy = new JsonArray();
    for (size_t i = 0; i < array.size(); i++) {
        if (array[i] != nullptr) {
            copy->addElement(array[i]->clone());
        } else {
            copy->addElement(nullptr);
        }
    }
    return copy;
}

void JsonArray::set(std::vector<std::string> path, JsonValue* newValue, unsigned depth) {
    if (depth >= path.size()) return;

    size_t index = 0;
    if (!parseIndex(path[depth], index) || index >= array.size() || array[index] == nullptr) {
        throw std::invalid_argument("Nonexisting or invalid index in JsonArray: " + path[depth]);
    }

    if (depth == path.size() - 1) {
        delete array[index];
        array[index] = newValue;
    } else {
        array[index]->set(path, newValue, depth + 1);
    }
}

void JsonArray::createPath(std::vector<std::string> path, JsonValue* newValue, unsigned depth) {
    if (depth >= path.size()) return;

    size_t index = 0;
    bool isValidNum = parseIndex(path[depth], index);

    if (isValidNum && index < array.size() && array[index] != nullptr) {
        if (depth == path.size() - 1) {
            delete array[index];
            array[index] = newValue;
        } else {
            array[index]->createPath(path, newValue, depth + 1);
        }
        return;
    }

    if (depth == path.size() - 1) {
        array.push_back(newValue);
    } else {
        JsonObject* newObject = new JsonObject();
        newObject->setName(path[depth]);
        array.push_back(newObject);
        newObject->createPath(path, newValue, depth + 1);
    }
}

void JsonArray::deletePath(std::vector<std::string> path, unsigned depth) {
    if (depth >= path.size()) {
        return;
    }

    size_t index = 0;
    // parseIndex проверява дали подаденият ключ е валидно число (напр. "0", "1")
    if (!parseIndex(path[depth], index) || index >= array.size() || array[index] == nullptr) {
        throw std::invalid_argument("Path or index not found in JsonArray: " + path[depth]);
    }

    // Ако сме на последния елемент от пътя — трием елемента от масива
    if (depth == path.size() - 1) {
        delete array[index];
        array.erase(array.begin() + index);
    } else {
        // Ако пътят продължава по-навътре (напр. масив от обекти: "users/0/name")
        array[index]->deletePath(path, depth + 1);
    }
}

void JsonArray::writeInFile(std::ostream& os) const {
    os << "[";
    for (size_t i = 0; i < array.size(); ++i) {
        if (array[i] != nullptr) {
            array[i]->writeInFile(os);
        }
        if (i < array.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
}

std::string JsonArray::toString() const {
    std::string res = "[";
    for (size_t i = 0; i < array.size(); i++) {
        if (array[i] != nullptr) {
            res += array[i]->toString();
        }
        if (i < array.size() - 1) {
            res += ", ";
        }
    }
    res += "]";
    return res;
}

JsonArray::~JsonArray() {
    for (size_t i = 0; i < array.size(); i++) {
        delete array[i];
    }
    array.clear();
}