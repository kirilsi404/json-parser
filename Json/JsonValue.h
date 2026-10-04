
#pragma once

#include <iostream>
class JsonValue {
public:
    virtual JsonValue* findPath(const std::vector<std::string>& path, unsigned depth);

    virtual void print(unsigned depth) const = 0;
    virtual std::string getName() const = 0;
    virtual std::vector<std::string> search(const std::string& key) const = 0;
    virtual JsonValue* clone() const = 0;
    virtual  void set(std::vector<std::string> path, JsonValue* newValue, unsigned depth) = 0;
    virtual void createPath(std::vector<std::string> path, JsonValue* newValue, unsigned depth) = 0;
    virtual void deletePath(std::vector<std::string> path, unsigned depth) = 0;
    virtual void writeInFile(std::ostream& os) const = 0;
    virtual std::string toString() const = 0;
    virtual ~JsonValue() = default;

};