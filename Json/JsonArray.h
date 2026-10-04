
#pragma once
#include  "JsonValue.h"

class JsonObject;
class JsonArray : public JsonValue {
    std::vector<JsonValue*> array;
public:
    //помощни функции
    void addElement(JsonValue* element);
    JsonValue* findPath(const std::vector<std::string>& path, unsigned depth) override;
    std::string getName() const override;
    JsonValue* clone() const override;
    std::string toString() const override;
    void writeInFile(std::ostream& os) const override;


    //основни функции
    void print(unsigned depth) const override;
    std::vector<std::string> search(const std::string& key) const override;
    void set(std::vector<std::string> path, JsonValue* newValue, unsigned depth) override;
    void createPath(std::vector<std::string> path, JsonValue* newValue, unsigned depth) override;
    void deletePath(std::vector<std::string> path, unsigned depth) override;
    ~JsonArray() override;
};


