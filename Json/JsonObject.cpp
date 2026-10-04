

#include "JsonObject.h"
#include "JsonArray.h"
void JsonObject::print(unsigned depth) const {
    std::cout << '{' << std::endl;
    for (int i = 0; i < pairs.size();i++) {
        if (pairs[i] == nullptr) {
            continue;
        }
        for (unsigned j = 0; j < depth + 1; j++) {
            std::cout << '\t';
        }
        std::cout << '"' << pairs[i]->name << '"' <<':';
        pairs[i]->value->print(depth + 1);
        if (i + 1 < pairs.size()) {
            std::cout << ',' << std::endl;
        }

    }
    for (unsigned j = 0; j < depth; j++) {
        std::cout << '\t';
    }
    std::cout << std::endl;
    std::cout << '}';
}

JsonValue * JsonObject::findPath(const std::vector<std::string> &path, unsigned depth) {
    if (depth == path.size()) {
        return this;
    }

    std::string currentKey = path[depth];
    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i] != nullptr && pairs[i]->name == currentKey) {
            return pairs[i]->value->findPath(path, depth + 1);
        }
    }
    return nullptr;
}

void JsonObject::addPair(std::string name, JsonValue* value) {
    Pair* newPair = new Pair;
    newPair->name = name;
    newPair->value = value;
    pairs.push_back(newPair);
}

void JsonObject::setName(const std::string &otherName) {
    name = otherName;
}

std::string JsonObject::getName() const {
    return name;
}

std::vector<std::string> JsonObject::search(const std::string &key) const {
    std::vector<std::string> result;
    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i] != nullptr && pairs[i]->value != nullptr) {
            if (key == pairs[i]->name) {
                result.push_back(pairs[i]->value->toString());
            }

            std::vector<std::string> innerResults = pairs[i]->value->search(key);
            for (size_t j = 0; j < innerResults.size(); j++) {
                result.push_back(innerResults[j]);
            }
        }
    }
    return result;
}

JsonValue* JsonObject::clone() const {
    return new JsonObject(*this);
}

void JsonObject::set(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    std::string curr = path[depth];

    for (int i = 0; i < pairs.size(); i++) {
        if (pairs[i]->name == curr) {
            if (depth == path.size() - 1) {
                delete pairs[i]->value;
                pairs[i]->value = newValue;
                return;
            }
            else {
                pairs[i]->value->set(path, newValue, depth + 1);
                return;
            }

        }
    }
    throw std::invalid_argument("Nonexisting path");



}

void JsonObject::createPath(std::vector<std::string> path, JsonValue *newValue, unsigned depth) {
    if (depth >= path.size()) {
        return;
    }

    std::string curr = path[depth];

    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i] != nullptr && pairs[i]->name == curr) {
            if (depth == path.size() - 1) {
                delete pairs[i]->value;
                pairs[i]->value = newValue;
                return;
            }

            JsonObject* asObject = dynamic_cast<JsonObject*>(pairs[i]->value);
            JsonArray* asArray = dynamic_cast<JsonArray*>(pairs[i]->value);

            if (!asObject && !asArray) {
                delete pairs[i]->value;
                pairs[i]->value = new JsonObject();
            }

            // Продължаваме рекурсивно по пътя
            pairs[i]->value->createPath(path, newValue, depth + 1);
            return;
        }
    }

    Pair* newPair = new Pair();
    newPair->name = curr;

    if (depth == path.size() - 1) {
        newPair->value = newValue;
        pairs.push_back(newPair);
    } else {
        JsonObject* intermediateObject = new JsonObject();
        newPair->value = intermediateObject;
        pairs.push_back(newPair);

        intermediateObject->createPath(path, newValue, depth + 1);
    }
}
void JsonObject::deletePath(std::vector<std::string> path, unsigned depth) {
    if (depth >= path.size()) {
        return;
    }

    std::string curr = path[depth];

    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i] != nullptr && pairs[i]->name == curr) {
            if (depth == path.size() - 1) {
                delete pairs[i]->value;
                delete pairs[i];
                pairs.erase(pairs.begin() + i);
                return;
            }

            if (pairs[i]->value == nullptr) {
                throw std::invalid_argument("Path points to null value at: " + curr);
            }
            pairs[i]->value->deletePath(path, depth + 1);
            return;
        }
    }

    throw std::invalid_argument("Path not found: " + curr);
}

void JsonObject::writeInFile(std::ostream &os) const {
    os << '{';

    for (size_t i = 0; i < pairs.size(); ++i) {
        os << '"' << pairs[i]->name << '"' << ' ' << ':';

        if (pairs[i]->value != nullptr) {
            pairs[i]->value->writeInFile(os);
        }

        if (i < pairs.size() - 1) {
            os << ", ";
        }
    }
    os << '}';
}
std::string JsonObject::toString() const {
    std::string res = "{";
    bool first = true;
    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i] != nullptr && pairs[i]->value != nullptr) {
            if (!first) {
                res += ",";
            }
            res += "\"" + pairs[i]->name + "\":" + pairs[i]->value->toString();
            first = false;
        }
    }
    res += "}";
    return res;
}

JsonObject::~JsonObject() {
    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i] != nullptr) {
            delete pairs[i]->value;
            delete pairs[i];
        }
    }
}
