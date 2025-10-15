#pragma once

#include <string>
#include <utility>

enum class ValueType {
    NUMBER,
    STRING,
    BOOLEAN,
    FLOAT,
    CHAR,
    NONE
};

struct Value {
    ValueType type;
    int i_value;
    std::string s_value;
    bool b_value;
    float f_value;
    char c_value;

    Value() : type(ValueType::NONE), i_value(0), b_value(false), f_value(0.0f), c_value('\0') {}
    explicit Value(int i) : type(ValueType::NUMBER), i_value(i), b_value(false), f_value(0.0f), c_value('\0') {}
    explicit Value(const std::string& s) : type(ValueType::STRING), s_value(s), i_value(0), b_value(false), f_value(0.0f), c_value('\0') {}
    explicit Value(bool b) : type(ValueType::BOOLEAN), b_value(b), i_value(0), f_value(0.0f), c_value('\0') {}
    explicit Value(float f) : type(ValueType::FLOAT), f_value(f), i_value(0), b_value(false), c_value('\0') {}
    explicit Value(char c) : type(ValueType::CHAR), c_value(c), i_value(0), b_value(false), f_value(0.0f) {}
};