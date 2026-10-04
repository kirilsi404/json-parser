cmake_minimum_required(VERSION 4.1)
project(JSONParcer)

set(CMAKE_CXX_STANDARD 20)

add_executable(JSONParcer main.cpp
        Json/JsonValue.h
        Json/JsonString.h
        Json/JsonNumber.h
        Json/JsonObject.h
        Json/JsonArray.h
        Json/JsonBool.h
        Json/JsonNull.h
        Commands/Commands.h
        Commands/Command.cpp
        Json/JsonParser.h
        Json/JsonArray.cpp
        Json/JsonBool.cpp
        Json/JsonNull.cpp
        Json/JsonNUmber.cpp
        Json/JsonObject.cpp
        Json/JsonParser.cpp
        Json/JsonString.cpp
        Tokenizer/Tokenizer.h
        Tokenizer/Tokenizer.cpp
        SplitString/SplitString.h
        SplitString/SplitString.cpp
        Json/JsonValue.cpp
)
