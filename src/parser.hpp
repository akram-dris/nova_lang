#pragma once
#include "lexer.hpp"
#include "value.hpp"
#include <vector>
#include <unordered_map>
#include <string>

#include <stdexcept>

class RuntimeError : public std::runtime_error {
public:
    explicit RuntimeError(const std::string& message) : std::runtime_error(message) {}
};

struct Parameter {
    std::string name;
    std::string type;
    bool has_default_value = false;
    Value default_value;

    // Explicit constructor
    Parameter(std::string name, std::string type, bool has_default_value = false, Value default_value = Value())
        : name(std::move(name)), type(std::move(type)), has_default_value(has_default_value), default_value(std::move(default_value)) {}
};

struct Function {
    std::string return_type;
    std::vector<Parameter> parameters;
    std::vector<Token> body;
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    void run();

private:
    std::vector<Token> tokens;
    size_t pos = 0;
    std::vector<std::unordered_map<std::string, Value>> scopes;
    std::unordered_map<std::string, Function> functions;
    bool is_returning = false;
    Value return_value;

    Token peek() const;
    Token peekNextToken() const;
    Token advance();

    void enterScope();
    void exitScope();
    Value getVariable(const std::string& name);
    void setVariable(const std::string& name, const Value& value);

    void handleShow();
    void handleVariableDeclaration();
    void handleIfStatement();
    void handleWhileStatement();
    void handleFunctionDeclaration();
    void handleReturnStatement();
    Value handleFunctionCall(const std::string& name);
    void handleAssignmentStatement();
    void parseBlock();
    bool parseCondition();
    Value parseFactor();
    Value parseTerm();
    Value parseExpression();
    void run_single_statement();
};