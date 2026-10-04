#pragma once
#include "../Json/JsonValue.h"
#include "../Json/JsonParser.h"
#include "../Tokenizer/Tokenizer.h"
class Commands {
    JsonValue *root;
    std::string fileName;
public:
    Commands() : root(nullptr), fileName(" "){}
    static std::vector<std::string> getPath(std::string);

    void start();
    void open(const std::string& file);
    void close();
    void save(std::vector<std::string> path);
    void saveAs(const std::string&  newFile, std::vector<std::string> path);
    void help();
    void exit();
    ~Commands();
};
