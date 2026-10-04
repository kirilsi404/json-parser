#include "Commands.h"
#include "../SplitString/SplitString.h"
#include <fstream>
#include <sstream>

// Превръща низ от типа "Files/File1" или "academic/grades/0" в път ["Files", "File1"]
std::vector<std::string> Commands::getPath(std::string line) {
    if (line.empty()) {
        throw std::invalid_argument("Path cannot be empty");
    }
    std::vector<std::string> result;
    int numberOfLevels = numberOfWords(line, '/');
    for (int i = 0; i < numberOfLevels; i++) {
        result.push_back(getWord(line, '/', i + 1));
    }
    return result;
}

// Помощна функция: премахва водещи и крайни интервали/табулации
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Помощна функция: извлича един аргумент (с или без кавички) и премества индекса
static std::string extractToken(const std::string& line, size_t& index) {
    while (index < line.size() && (line[index] == ' ' || line[index] == '\t')) {
        index++;
    }
    if (index >= line.size()) return "";

    std::string token;
    if (line[index] == '"') {
        index++; // прескачаме отварящата кавичка
        while (index < line.size() && line[index] != '"') {
            token += line[index++];
        }
        if (index < line.size()) index++; // прескачаме затварящата кавичка
    } else {
        while (index < line.size() && line[index] != ' ' && line[index] != '\t') {
            token += line[index++];
        }
    }
    return token;
}

void Commands::start() {
    bool isRunning = true;
    std::cout << "The program is running." << std::endl;
    std::cout << "For more info on the commands type: help" << std::endl;

    std::string line;

    while (isRunning) {
        if (!std::getline(std::cin, line)) {
            break;
        }

        line = trim(line);
        if (line.empty()) {
            continue;
        }

        size_t idx = 0;
        std::string commandName = extractToken(line, idx);

        if (commandName == "exit") {
            exit();
            isRunning = false;
        }
        else if (commandName == "open") {
            std::string fileToOpen = extractToken(line, idx);
            if (fileToOpen.empty()) {
                std::cout << "Error: Please specify a file name to open!" << std::endl;
                continue;
            }
            open(fileToOpen);
        }
        else if (commandName == "close") {
            close();
        }
        else if (commandName == "save") {
            if (root == nullptr) {
                std::cout << "Error: No file is currently opened!" << std::endl;
                continue;
            }

            std::string pathStr = extractToken(line, idx);
            std::vector<std::string> path;
            if (!pathStr.empty()) {
                try {
                    path = getPath(pathStr);
                } catch (const std::exception& e) {
                    std::cout << "Error: Invalid path: " << e.what() << std::endl;
                    continue;
                }
            }
            save(path);
        }
        else if (commandName == "saveAs") {
            if (root == nullptr) {
                std::cout << "Error: No file is currently opened!" << std::endl;
                continue;
            }

            std::string newFileName = extractToken(line, idx);
            if (newFileName.empty()) {
                std::cout << "Error: Please specify a destination file name!" << std::endl;
                continue;
            }

            std::string pathStr = extractToken(line, idx);
            std::vector<std::string> path;
            if (!pathStr.empty()) {
                try {
                    path = getPath(pathStr);
                } catch (const std::exception& e) {
                    std::cout << "Error: Invalid path: " << e.what() << std::endl;
                    continue;
                }
            }
            saveAs(newFileName, path);
        }
        else if (commandName == "help") {
            help();
        }
        else if (commandName == "print") {
            if (root != nullptr) {
                root->print(0);
                std::cout << std::endl;
            } else {
                std::cout << "No file is currently opened." << std::endl;
            }
        }
        else if (commandName == "search") {
            if (root == nullptr) {
                std::cout << "Error: No file is currently opened!" << std::endl;
                continue;
            }

            std::string searchKey = extractToken(line, idx);
            if (searchKey.empty()) {
                std::cout << "Error: Please specify a key to search for!" << std::endl;
                continue;
            }

            std::vector<std::string> result = root->search(searchKey);
            if (result.empty()) {
                std::cout << "No matches found for key: " << searchKey << std::endl;
            } else {
                for (size_t i = 0; i < result.size(); i++) {
                    std::cout << result[i] << (i + 1 < result.size() ? "," : "") << std::endl;
                }
            }
        }
        else if (commandName == "set") {
            if (root == nullptr) {
                std::cout << "Error: No file is currently opened!" << std::endl;
                continue;
            }

            std::string pathStr = extractToken(line, idx);
            // Остатъкът от реда е суровата JSON стойност (напр. "text", 42, true, {"a":1})
            std::string rawValueStr = trim(line.substr(idx));

            if (pathStr.empty() || rawValueStr.empty()) {
                std::cout << "Error: Invalid syntax! Usage: set <path> <value>" << std::endl;
                continue;
            }

            std::vector<std::string> path;
            try {
                path = getPath(pathStr);
            } catch (const std::exception& e) {
                std::cout << "Error: Invalid path syntax: " << e.what() << std::endl;
                continue;
            }

            JsonValue* newValue = nullptr;
            try {
                Tokenizer tokenizer(rawValueStr);
                std::vector<Token> tokens = tokenizer.tokenize();
                JsonParser parser(tokens);
                newValue = parser.parseValue();
            } catch (const std::exception& e) {
                std::cout << "Error: Invalid new value JSON syntax: " << e.what() << std::endl;
                continue;
            }

            try {
                root->set(path, newValue, 0);
                std::cout << "Successfully set value at " << pathStr << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error while setting value: " << e.what() << std::endl;
                delete newValue; // Предотвратява memory leak
            }
        }
        else if (commandName == "deletePath") {
            if (root == nullptr) {
                std::cout << "Error: No file is currently opened!" << std::endl;
                continue;
            }

            std::string pathStr = extractToken(line, idx);
            if (pathStr.empty()) {
                std::cout << "Error: No path entered!" << std::endl;
                continue;
            }

            try {
                std::vector<std::string> path = getPath(pathStr);
                root->deletePath(path, 0);
                std::cout << "Successfully deleted value at: " << pathStr << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error while deleting: " << e.what() << std::endl;
            }
        }
        else if (commandName == "create") {
            if (root == nullptr) {
                std::cout << "Error: You must open a file first!" << std::endl;
                continue;
            }

            std::string pathStr = extractToken(line, idx);
            std::string rawValueStr = trim(line.substr(idx));

            if (pathStr.empty() || rawValueStr.empty()) {
                std::cout << "Error: Invalid syntax! Usage: create <path> <value>" << std::endl;
                continue;
            }

            std::vector<std::string> path;
            try {
                path = getPath(pathStr);
            } catch (const std::exception& e) {
                std::cout << "Error: Invalid path syntax: " << e.what() << std::endl;
                continue;
            }

            JsonValue* newValue = nullptr;
            try {
                Tokenizer tokenizer(rawValueStr);
                std::vector<Token> tokens = tokenizer.tokenize();
                JsonParser parser(tokens);
                newValue = parser.parseValue();
            } catch (const std::exception& e) {
                std::cout << "Error: Invalid new value JSON syntax: " << e.what() << std::endl;
                continue;
            }

            try {
                root->createPath(path, newValue, 0);
                std::cout << "Successfully created new value at " << pathStr << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error while creating path: " << e.what() << std::endl;
                delete newValue; // Предотвратява memory leak
            }
        }
        else {
            std::cout << "Unknown command: " << commandName << ". Type 'help' for available commands." << std::endl;
        }
    }
}

void Commands::open(const std::string& file) {
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }

    fileName = file;
    std::ifstream ifs(fileName);

    if (!ifs.is_open()) {
        std::cout << "File not found. Creating a new empty JSON object." << std::endl;
        root = new JsonObject();
        return;
    }

    std::string content;
    std::string currentLine;
    while (std::getline(ifs, currentLine)) {
        content += currentLine + "\n";
    }
    ifs.close();

    content = trim(content);
    if (content.empty()) {
        root = new JsonObject();
        std::cout << "Opened empty file." << std::endl;
        return;
    }

    try {
        Tokenizer tokenizer(content);
        std::vector<Token> tokens = tokenizer.tokenize();
        JsonParser parser(tokens);
        root = parser.parse();
        std::cout << "Successfully opened " << fileName << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error while parsing file: " << e.what() << std::endl;
        root = nullptr;
        fileName = "File not opened";
    }
}

void Commands::close() {
    if (root == nullptr) {
        std::cout << "Error: No file is currently opened." << std::endl;
        return;
    }
    delete root;
    root = nullptr;
    std::cout << "Successfully closed " << fileName << std::endl;
    fileName = "File not opened";
}

void Commands::save(std::vector<std::string> path) {
    if (root == nullptr) {
        std::cout << "Error: No file is currently opened!" << std::endl;
        return;
    }

    std::ofstream ofs(fileName);
    if (!ofs.is_open()) {
        std::cout << "Error: Could not open file for writing: " << fileName << std::endl;
        return;
    }

    if (path.empty()) {
        root->writeInFile(ofs);
        std::cout << "Successfully saved to " << fileName << std::endl;
    } else {
        JsonValue* subTree = root->findPath(path, 0);
        if (subTree == nullptr) {
            std::cout << "Error: Path not found in JSON structure!" << std::endl;
            return;
        }
        subTree->writeInFile(ofs);
        std::cout << "Successfully saved subpath to " << fileName << std::endl;
    }
}

void Commands::saveAs(const std::string& newFile, std::vector<std::string> path) {
    if (root == nullptr) {
        std::cout << "Error: No file is currently opened!" << std::endl;
        return;
    }

    std::ofstream ofs(newFile);
    if (!ofs.is_open()) {
        std::cout << "Error: Cannot create or open file: " << newFile << std::endl;
        return;
    }

    if (path.empty()) {
        root->writeInFile(ofs);
        std::cout << "Successfully saved in " << newFile << std::endl;
    } else {
        JsonValue* subTree = root->findPath(path, 0);
        if (subTree == nullptr) {
            std::cout << "Error: Path not found in JSON structure!" << std::endl;
            return;
        }
        subTree->writeInFile(ofs);
        std::cout << "Successfully saved subpath in " << newFile << std::endl;
    }
}

void Commands::help() {
    std::cout <<
        "SYNTAX RULES:\n"
        "  <path>   Levels are separated by '/' (e.g. profile/faculty_number, courses/0).\n"
        "  <value>  Strings must be quoted (\"text\"), while numbers (42), booleans (true/false),\n"
        "           null, arrays ([1, 2]), and objects ({\"k\": 1}) are written without outer quotes.\n\n"
        "COMMANDS:\n"
        "  open <file>            Loads a JSON file (e.g. open test.json)\n"
        "  close                  Closes the current file\n"
        "  print                  Prints the JSON structure to the screen\n"
        "  save [<path>]          Saves the entire file or a subpath (e.g. save academic/grades)\n"
        "  saveAs <file> [<path>] Saves the document or a subpath to a new file (e.g. saveAs new.json)\n"
        "  search <key>           Finds all occurrences matching the key (e.g. search faculty_number)\n"
        "  set <path> <value>     Updates an existing value (e.g. set student \"Georgi Georgiev\")\n"
        "  create <path> <value>  Creates a new key and hierarchy (e.g. create student/grades/0 6)\n"
        "  deletePath <path>      Deletes an element by path (e.g. deletePath profile/is_first_year)\n"
        "  help                   Displays this guide\n"
        "  exit                   Exits the program\n";

    std::cout << "To change/set a specific value in an array you use the index of said value(e.g. set academic/grades/0 6)* ";
}

void Commands::exit() {
    std::cout << "You have exited the program." << std::endl;
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }
}

Commands::~Commands() {
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }
}
