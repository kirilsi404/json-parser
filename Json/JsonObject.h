
#pragma once
#include "JsonValue.h"

class JsonObject : public JsonValue {
    std::string name;
    struct Pair {
        std::string name;
        JsonValue* value;
    };
    std::vector<Pair*> pairs;
public:
    JsonObject(){}
    //помощни функции
    JsonValue* findPath(const std::vector<std::string>& path, unsigned depth) override;
    void addPair(std::string name, JsonValue* value);
    void setName(const std::string& otherName);
    std::string getName() const override;
    JsonValue* clone() const override;
    void writeInFile(std::ostream& os) const override;
    std::string toString() const override;


    //основни функции
    void print(unsigned depth) const override;
    std::vector<std::string> search(const std::string& key) const override;
    void set(std::vector<std::string> path, JsonValue* newValue, unsigned depth) override;
    void createPath(std::vector<std::string> path, JsonValue* newValue, unsigned depth) override;
    void deletePath(std::vector<std::string> path, unsigned depth) override;
    ~JsonObject() override;

};