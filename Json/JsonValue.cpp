#include "JsonValue.h"

JsonValue* JsonValue::findPath(const std::vector<std::string> &path, unsigned depth) {
    if (depth == path.size()) {
        return this;
    }
    return nullptr;
}

