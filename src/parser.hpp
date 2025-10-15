#pragma once
#include "lexer.hpp"
#include "value.hpp"
#include <vector>
#include <unordered_map>
#include <string>

struct Parameter {
    std::string name;
    std::string type;
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

    void handlePrint();
    void handleVariableDeclaration();
    void handleIfStatement();
    void handleWhileStatement();
    void handleFunctionDeclaration();
    void handleReturnStatement();
    Value handleFunctionCall(const std::string& name);
    void parseBlock();
    bool parseCondition();
    Value parseFactor();
    Value parseTerm();
    Value parseExpression();
    void run_single_statement();
};