


#pragma once
#include "JsonValue.h"

class JsonNumber : public JsonValue {
    int number;
public:
    //помощни функции
    JsonNumber(int num) : number(num){}
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
    ~JsonNumber() override = default;

};