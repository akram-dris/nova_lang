#pragma once

#include <string>
#include <utility>

enum class ValueType {
    NUMBER,
    STRING,
    BOOLEAN,
    NIL // for uninitialized values
};

struct Value {
    ValueType type;
    std::string s_value; // for strings
    int i_value; // for numbers
    bool b_value; // for booleans

    Value() : type(ValueType::NIL), i_value(0) {}
    explicit Value(int val) : type(ValueType::NUMBER), i_value(val) {}
    explicit Value(std::string val) : type(ValueType::STRING), s_value(std::move(val)) {}
    explicit Value(bool val) : type(ValueType::BOOLEAN), b_value(val) {}
};