//
// Created by Kiril Svetlozarov Ivanov on 20.05.26.
//

#pragma once
#include <iostream>
#include "JsonValue.h"

class JsonString : public JsonValue {
    std::string str;
public:
    //помощни функции
    JsonString(std::string value) : str(value){}
    std::string getName() const override;
    std::vector<std::string> search(const std::string& key) const override;
    JsonValue* clone() const override;
    void writeInFile(std::ostream& os) const override;
    std::string toString() const override;

    //основни функции
    void print(unsigned depth) const override;
    void set(std::vector<std::string> path, JsonValue* newValue, unsigned depth) override;
    void createPath(std::vector<std::string> path, JsonValue* newValue, unsigned depth) override;
    void deletePath(std::vector<std::string> path, unsigned depth) override;
    ~JsonString() override = default;
};


