#include "parser.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {
    enterScope(); // Global scope
}

Token Parser::peek() const {
    if (pos >= tokens.size()) return { TokenType::END_OF_FILE, "" };
    return tokens[pos];
}

Token Parser::peekNextToken() const {
    if (pos + 1 >= tokens.size()) return { TokenType::END_OF_FILE, "" };
    return tokens[pos + 1];
}

Token Parser::advance() {
    if (pos < tokens.size()) return tokens[pos++];
    return { TokenType::END_OF_FILE, "" };
}

void Parser::enterScope() {
    scopes.emplace_back();
}

void Parser::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

void Parser::setVariable(const std::string& name, const Value& value) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->count(name)) {
            (*it)[name] = value;
            return;
        }
    }
    // If not found in any parent scope, set in current scope
    if (!scopes.empty()) {
        scopes.back()[name] = value;
    }
}

Value Parser::getVariable(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->count(name)) {
            return (*it)[name];
        }
    }
    std::cerr << "Error: undefined variable '" << name << "'\n";
    return Value();
}

bool Parser::parseCondition() {
    Value result = parseExpression();
    if (result.type == ValueType::BOOLEAN) {
        return result.b_value;
    } else if (result.type == ValueType::NUMBER) {
        return result.i_value != 0;
    } else if (result.type == ValueType::STRING) {
        return !result.s_value.empty();
    }
    std::cerr << "Error: condition must be a boolean, number, or string.\n";
    return false;
}

Value Parser::parseFactor() {
    Token token = advance();
    if (token.type == TokenType::NUMBER) {
        return Value(std::stoi(token.value));
    } else if (token.type == TokenType::STRING) {
        return Value(token.value);
    } else if (token.type == TokenType::KEYWORD_TRUE) {
        return Value(true);
    } else if (token.type == TokenType::KEYWORD_FALSE) {
        return Value(false);
    }
    else if (token.type == TokenType::IDENTIFIER) {
        if (peek().type == TokenType::LEFT_PAREN) {
            return handleFunctionCall(token.value);
        }
        return getVariable(token.value);
    } else {
        std::cerr << "Syntax error: expected number, string, boolean, or identifier\n";
        return Value();
    }
}

Value Parser::parseTerm() {
    Value result = parseFactor();

    while (peek().type == TokenType::STAR || peek().type == TokenType::SLASH) {
        Token op = advance();
        Value rhs = parseFactor();

        if (result.type != ValueType::NUMBER || rhs.type != ValueType::NUMBER) {
            std::cerr << "Error: arithmetic operations can only be performed on numbers.\n";
            return Value();
        }

        if (op.type == TokenType::STAR) {
            result.i_value *= rhs.i_value;
        } else if (op.type == TokenType::SLASH) {
            if (rhs.i_value == 0) {
                std::cerr << "Error: Division by zero.\n";
                return Value();
            }
            result.i_value /= rhs.i_value;
        }
    }
    return result;
}

Value Parser::parseExpression() {
    if (peek().type == TokenType::KEYWORD_TRUE || peek().type == TokenType::KEYWORD_FALSE) {
        return parseFactor();
    }
    
    Value result = parseTerm();

    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS || peek().type == TokenType::EQUAL_EQUAL || peek().type == TokenType::NOT_EQUAL || peek().type == TokenType::GREATER || peek().type == TokenType::GREATER_EQUAL || peek().type == TokenType::LESS || peek().type == TokenType::LESS_EQUAL) {
        Token op = advance();
        Value rhs = parseTerm();

        if (op.type == TokenType::PLUS) {
            if (result.type == ValueType::NUMBER && rhs.type == ValueType::NUMBER) {
                result.i_value += rhs.i_value;
            } else if (result.type == ValueType::STRING && rhs.type == ValueType::STRING) {
                result.s_value += rhs.s_value;
            } else {
                std::cerr << "Error: invalid operands for + operator.\n";
                return Value();
            }
        } else if (op.type == TokenType::MINUS) {
            if (result.type == ValueType::NUMBER && rhs.type == ValueType::NUMBER) {
                result.i_value -= rhs.i_value;
            } else {
                std::cerr << "Error: invalid operands for - operator.\n";
                return Value();
            }
        } else { 
             if (result.type != ValueType::NUMBER || rhs.type != ValueType::NUMBER) {
                std::cerr << "Error: comparison can only be performed on numbers.\n";
                return Value(false);
            }
            if (op.type == TokenType::EQUAL_EQUAL) {
                return Value(result.i_value == rhs.i_value);
            } else if (op.type == TokenType::NOT_EQUAL) {
                return Value(result.i_value != rhs.i_value);
            } else if (op.type == TokenType::GREATER) {
                return Value(result.i_value > rhs.i_value);
            } else if (op.type == TokenType::GREATER_EQUAL) {
                return Value(result.i_value >= rhs.i_value);
            } else if (op.type == TokenType::LESS) {
                return Value(result.i_value < rhs.i_value);
            } else if (op.type == TokenType::LESS_EQUAL) {
                return Value(result.i_value <= rhs.i_value);
            }
        }
    }
    return result;
}

void Parser::handleShow() {
    advance(); // skip 'print'
    Value value = parseExpression();
    if (value.type == ValueType::NUMBER) {
        std::cout << value.i_value << std::endl;
    } else if (value.type == ValueType::STRING) {
        std::cout << value.s_value << std::endl;
    } else if (value.type == ValueType::BOOLEAN) {
        std::cout << (value.b_value ? "true" : "false") << std::endl;
    }
}

void Parser::handleVariableDeclaration() {
    Token name = advance(); // consume the identifier
    advance(); // consume the ':'
    Token type = advance();
    if (type.type != TokenType::IDENTIFIER && type.type != TokenType::KEYWORD_NUM && type.type != TokenType::KEYWORD_STRING && type.type != TokenType::KEYWORD_BOOL) {
        std::cerr << "Syntax error: expected type annotation\n";
        return;
    }
    if (advance().type != TokenType::EQUAL) {
        std::cerr << "Syntax error: expected '=' after type annotation\n";
        return;
    }
    Value value = parseExpression();
    setVariable(name.value, value);
}

void Parser::handleAssignmentStatement() {
    Token name = advance(); // consume the identifier (variable name)
    advance(); // consume the '='
    Value value = parseExpression();
    setVariable(name.value, value);
}

void Parser::parseBlock() {
    enterScope();
    while (peek().type != TokenType::KEYWORD_END && peek().type != TokenType::END_OF_FILE) {
        if (is_returning) break;
        run_single_statement();
    }
    if (peek().type == TokenType::KEYWORD_END) {
        advance(); // consume 'end'
    }
    exitScope();
}

void Parser::handleIfStatement() {
    advance(); // skip 'if'
    Value condition = parseExpression();
    if (advance().type != TokenType::KEYWORD_START) {
        std::cerr << "Syntax error: expected 'start' after condition\n";
        return;
    }

    bool is_true = false;
    if (condition.type == ValueType::BOOLEAN) {
        is_true = condition.b_value;
    } else if (condition.type == ValueType::NUMBER) {
        is_true = (condition.i_value != 0);
    } else if (condition.type == ValueType::STRING) {
        is_true = !condition.s_value.empty();
    }

    if (is_true) {
        parseBlock();
        if (peek().type == TokenType::KEYWORD_ELSE) {
            advance(); // skip 'else'
            if (advance().type != TokenType::KEYWORD_START) {
                std::cerr << "Syntax error: expected 'start' after 'else'\n";
                return;
            }
            // skip the else block
            int start_level = 1;
            while (start_level > 0 && peek().type != TokenType::END_OF_FILE) {
                Token t = advance();
                if (t.type == TokenType::KEYWORD_START) start_level++;
                if (t.type == TokenType::KEYWORD_END) start_level--;
            }
        }
    } else {
        // skip the if block
        int start_level = 1;
        while (start_level > 0 && peek().type != TokenType::END_OF_FILE) {
            Token t = advance();
            if (t.type == TokenType::KEYWORD_START) start_level++;
            if (t.type == TokenType::KEYWORD_END) start_level--;
        }
        if (peek().type == TokenType::KEYWORD_ELSE) {
            advance(); // skip 'else'
            if (advance().type != TokenType::KEYWORD_START) {
                std::cerr << "Syntax error: expected 'start' after 'else'\n";
                return;
            }
            parseBlock();
        }
    }
}

void Parser::handleWhileStatement() {
    advance(); // skip 'while'
    
    // Store the current position to jump back to for the loop
    size_t condition_start_pos = pos;

    // Keep looping as long as the condition is true
    while (true) {
        // Reset position to evaluate the condition
        pos = condition_start_pos;
        if (!parseCondition()) {
            break; // Exit loop if condition is false
        }

        if (peek().type != TokenType::KEYWORD_START) {
            std::cerr << "Syntax error: expected 'start' after while condition\n";
            return;
        }
        advance(); // skip 'start'

        parseBlock();

        if (is_returning) { // Handle return inside loop
            return;
        }
    }

    // After the loop, skip the condition and the block
    // First, skip the condition part
    pos = condition_start_pos;
    // Find the matching 'start' for the block
    while (peek().type != TokenType::KEYWORD_START && peek().type != TokenType::END_OF_FILE) {
        advance();
    }

    if (peek().type == TokenType::KEYWORD_START) {
        advance(); // skip 'start'
        int start_level = 1;
        while (start_level > 0 && peek().type != TokenType::END_OF_FILE) {
            Token t = advance();
            if (t.type == TokenType::KEYWORD_START) start_level++;
            if (t.type == TokenType::KEYWORD_END) start_level--;
        }
    }
}

void Parser::handleFunctionDeclaration() {
    advance(); // skip 'fun'
    if (advance().type != TokenType::COLON) {
        std::cerr << "Syntax error: expected ':' after 'fun'\n";
        return;
    }
    Token return_type = advance();
    if (return_type.type != TokenType::IDENTIFIER && return_type.type != TokenType::KEYWORD_STRING && return_type.type != TokenType::KEYWORD_NUM && return_type.type != TokenType::KEYWORD_BOOL) {
        std::cerr << "Syntax error: expected return type\n";
        return;
    }

    Token name = advance();
    if (name.type != TokenType::IDENTIFIER) {
        std::cerr << "Syntax error: expected function name\n";
        return;
    }

    Function func;
    func.return_type = return_type.value;

    while (peek().type != TokenType::KEYWORD_START && peek().type != TokenType::END_OF_FILE) {
        Token param_name = advance();
        if (param_name.type != TokenType::IDENTIFIER) {
            std::cerr << "Syntax error: expected parameter name\n";
            return;
        }
        if (advance().type != TokenType::COLON) {
            std::cerr << "Syntax error: expected ':' after parameter name\n";
            return;
        }
        Token param_type = advance();
        if (param_type.type != TokenType::IDENTIFIER && param_type.type != TokenType::KEYWORD_STRING && param_type.type != TokenType::KEYWORD_NUM && param_type.type != TokenType::KEYWORD_BOOL) {
            std::cerr << "Syntax error: expected parameter type\n";
            return;
        }
        func.parameters.push_back({param_name.value, param_type.value});
    }

    if (peek().type != TokenType::KEYWORD_START) {
        std::cerr << "Syntax error: expected 'start' before function body\n";
        return;
    }
    advance(); // consume 'start'

    int start_level = 1;
    while (start_level > 0 && peek().type != TokenType::END_OF_FILE) {
        func.body.push_back(peek());
        if (peek().type == TokenType::KEYWORD_START) start_level++;
        if (peek().type == TokenType::KEYWORD_END) start_level--;
        advance();
    }
    if (!func.body.empty()) {
        func.body.pop_back(); // remove the last 'end'
    }

    functions[name.value] = func;
}

void Parser::handleReturnStatement() {
    advance(); // skip 'return'
    return_value = parseExpression();
    is_returning = true;
}

Value Parser::handleFunctionCall(const std::string& name) {
    advance(); // skip '('
    if (!functions.count(name)) {
        std::cerr << "Error: undefined function '" << name << "'\n";
        return Value();
    }

    Function func = functions[name];
    std::vector<Value> args;
    while (peek().type != TokenType::RIGHT_PAREN) {
        args.push_back(parseExpression());
        if (peek().type == TokenType::COMMA) {
            advance();
        }
    }
    advance(); // skip ')'

    if (args.size() != func.parameters.size()) {
        std::cerr << "Error: incorrect number of arguments for function '" << name << "'\n";
        return Value();
    }

    // Create a new parser for the function body
    Parser func_parser(func.body);
    func_parser.functions = functions;

    // Set up the function's local variables
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        func_parser.setVariable(func.parameters[i].name, args[i]);
    }

    // Execute the function body
    func_parser.run();

    if (func_parser.is_returning) {
        return func_parser.return_value;
    }

    return Value();
}

void Parser::run_single_statement() {
    if (is_returning) return;
    Token current = peek();
    if (current.type == TokenType::SHOW) {
        handleShow();
    } else if (current.type == TokenType::KEYWORD_IF) {
        handleIfStatement();
    } else if (current.type == TokenType::KEYWORD_WHILE) {
        handleWhileStatement();
    } else if (current.type == TokenType::KEYWORD_FUN) {
        handleFunctionDeclaration();
    } else if (current.type == TokenType::KEYWORD_RETURN) {
        handleReturnStatement();
    } else if (current.type == TokenType::IDENTIFIER) {
        if (peekNextToken().type == TokenType::COLON) {
            handleVariableDeclaration();
        } else if (peekNextToken().type == TokenType::EQUAL) {
            handleAssignmentStatement();
        } else {
            // Expression statement (e.g., function call or just an identifier)
            parseExpression();
        }
    } else {
        // Fallback for other expression statements that don't start with IDENTIFIER
        parseExpression();
    }
}

void Parser::run() {
    while (peek().type != TokenType::END_OF_FILE) {
        if (is_returning) return;
        run_single_statement();
    }
}