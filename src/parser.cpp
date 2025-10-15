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
    throw RuntimeError("Undefined variable '" + name + "'");
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
    throw RuntimeError("Condition must be a boolean, number, or string.");
}

Value Parser::parseFactor() {

    Token token = advance();

    if (token.type == TokenType::NUMBER) {

        if (token.value.find('.') != std::string::npos) {

            return Value(std::stof(token.value));

        } else {

            return Value(std::stoi(token.value));

        }

    } else if (token.type == TokenType::STRING) {

        return Value(token.value);

    } else if (token.type == TokenType::KEYWORD_TRUE) {

        return Value(true);

    } else if (token.type == TokenType::KEYWORD_FALSE) {

        return Value(false);

        } else if (token.type == TokenType::CHAR) {

            return Value(token.value[0]);

        } else if (token.type == TokenType::IDENTIFIER) {
            // Check if the identifier is a known function
            if (functions.count(token.value)) {
                return handleFunctionCall(token.value);
            }
            // Otherwise, it's a variable
            return getVariable(token.value);
        } else {

            throw RuntimeError("Syntax error: expected number, string, boolean, char, or identifier. Received token type: " + std::to_string(static_cast<int>(token.type)));

        }
}

Value Parser::parseTerm() {
    Value result = parseFactor();

    while (peek().type == TokenType::STAR || peek().type == TokenType::SLASH) {
        Token op = advance();
        Value rhs = parseFactor();

        if ((result.type != ValueType::NUMBER && result.type != ValueType::FLOAT) || (rhs.type != ValueType::NUMBER && rhs.type != ValueType::FLOAT)) {
                    throw RuntimeError("Arithmetic operations can only be performed on numbers or floats.");        }

        // Promote to float if either operand is float
        bool promote_to_float = (result.type == ValueType::FLOAT || rhs.type == ValueType::FLOAT);
        float f_result = (promote_to_float && result.type == ValueType::NUMBER) ? static_cast<float>(result.i_value) : result.f_value;
        float f_rhs = (promote_to_float && rhs.type == ValueType::NUMBER) ? static_cast<float>(rhs.i_value) : rhs.f_value;

        if (op.type == TokenType::STAR) {
            if (promote_to_float) {
                result = Value(f_result * f_rhs);
            } else {
                result.i_value *= rhs.i_value;
            }
        } else if (op.type == TokenType::SLASH) {
            if (promote_to_float) {
                if (f_rhs == 0.0f) {
                    throw RuntimeError("Division by zero.");
                }
                result = Value(f_result / f_rhs);
            } else {
                if (rhs.i_value == 0) {
                    throw RuntimeError("Division by zero.");
                }
                result.i_value /= rhs.i_value;
            }
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
            } else if (result.type == ValueType::FLOAT && rhs.type == ValueType::FLOAT) {
                result.f_value += rhs.f_value;
            } else if (result.type == ValueType::NUMBER && rhs.type == ValueType::FLOAT) {
                result.f_value = static_cast<float>(result.i_value) + rhs.f_value;
                result.type = ValueType::FLOAT;
            } else if (result.type == ValueType::FLOAT && rhs.type == ValueType::NUMBER) {
                result.f_value += static_cast<float>(rhs.i_value);
            } else if (result.type == ValueType::STRING && rhs.type == ValueType::STRING) {
                result.s_value += rhs.s_value;
            } else {
                throw RuntimeError("Invalid operands for + operator.");
            }
        } else if (op.type == TokenType::MINUS) {
            if (result.type == ValueType::NUMBER && rhs.type == ValueType::NUMBER) {
                result.i_value -= rhs.i_value;
            } else if (result.type == ValueType::FLOAT && rhs.type == ValueType::FLOAT) {
                result.f_value -= rhs.f_value;
            } else if (result.type == ValueType::NUMBER && rhs.type == ValueType::FLOAT) {
                result.f_value = static_cast<float>(result.i_value) - rhs.f_value;
                result.type = ValueType::FLOAT;
            } else if (result.type == ValueType::FLOAT && rhs.type == ValueType::NUMBER) {
                result.f_value -= static_cast<float>(rhs.i_value);
            } else {
                throw RuntimeError("Invalid operands for - operator.");
            }
        } else { 
            // Comparison operators
            if ((result.type != ValueType::NUMBER && result.type != ValueType::FLOAT) || (rhs.type != ValueType::NUMBER && rhs.type != ValueType::FLOAT)) {
                throw RuntimeError("Comparison can only be performed on numbers or floats.");
            }

            bool promote_to_float = (result.type == ValueType::FLOAT || rhs.type == ValueType::FLOAT);
            float f_result = (promote_to_float && result.type == ValueType::NUMBER) ? static_cast<float>(result.i_value) : result.f_value;
            float f_rhs = (promote_to_float && rhs.type == ValueType::NUMBER) ? static_cast<float>(rhs.i_value) : rhs.f_value;

            if (op.type == TokenType::EQUAL_EQUAL) {
                return Value(f_result == f_rhs);
            } else if (op.type == TokenType::NOT_EQUAL) {
                return Value(f_result != f_rhs);
            } else if (op.type == TokenType::GREATER) {
                return Value(f_result > f_rhs);
            } else if (op.type == TokenType::GREATER_EQUAL) {
                return Value(f_result >= f_rhs);
            } else if (op.type == TokenType::LESS) {
                return Value(f_result < f_rhs);
            } else if (op.type == TokenType::LESS_EQUAL) {
                return Value(f_result <= f_rhs);
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
    } else if (value.type == ValueType::FLOAT) {
        std::cout << value.f_value << std::endl;
    } else if (value.type == ValueType::CHAR) {
        std::cout << value.c_value << std::endl;
    }
}

void Parser::handleVariableDeclaration() {
    Token name = advance(); // consume the identifier
    advance(); // consume the ':'
    Token type = advance();
    if (type.type != TokenType::IDENTIFIER && type.type != TokenType::KEYWORD_NUM && type.type != TokenType::KEYWORD_STRING && type.type != TokenType::KEYWORD_BOOL && type.type != TokenType::KEYWORD_FLOAT && type.type != TokenType::KEYWORD_CHAR) {
        throw RuntimeError("Syntax error: expected type annotation");
    }
    if (advance().type != TokenType::EQUAL) {
        throw RuntimeError("Syntax error: expected '=' after type annotation");
    }
    Value value = parseExpression();
    
    // Type checking and conversion
    if (type.type == TokenType::KEYWORD_NUM) {
        if (value.type == ValueType::NUMBER) {
            setVariable(name.value, value);
        } else if (value.type == ValueType::FLOAT) {
            setVariable(name.value, Value(static_cast<int>(value.f_value)));
        } else {
            throw RuntimeError("Error: cannot convert to num");
        }
    } else if (type.type == TokenType::KEYWORD_STRING) {
        if (value.type == ValueType::STRING) {
            setVariable(name.value, value);
        } else {
            throw RuntimeError("Error: cannot convert to string");
        }
    } else if (type.type == TokenType::KEYWORD_BOOL) {
        if (value.type == ValueType::BOOLEAN) {
            setVariable(name.value, value);
        } else {
            throw RuntimeError("Error: cannot convert to bool");
        }
    } else if (type.type == TokenType::KEYWORD_FLOAT) {
        if (value.type == ValueType::FLOAT) {
            setVariable(name.value, value);
        } else if (value.type == ValueType::NUMBER) {
            setVariable(name.value, Value(static_cast<float>(value.i_value)));
        } else {
            throw RuntimeError("Error: cannot convert to float");
        }
    } else if (type.type == TokenType::KEYWORD_CHAR) {
        if (value.type == ValueType::CHAR) {
            setVariable(name.value, value);
        } else {
            throw RuntimeError("Error: cannot convert to char");
        }
    } else {
        // For IDENTIFIER type (e.g., custom types), direct assignment for now
        setVariable(name.value, value);
    }
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
        throw RuntimeError("Syntax error: expected 'start' after condition");
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
            // The else block does not have its own 'start' keyword
            // It directly follows 'else' and is terminated by the 'end' of the if statement
            parseBlock();
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
            // The else block does not have its own 'start' keyword
            // It directly follows 'else' and is terminated by the 'end' of the if statement
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
            throw RuntimeError("Syntax error: expected 'start' after while condition");
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
        throw RuntimeError("Syntax error: expected ':' after 'fun'");
    }
    Token return_type = advance();
    if (return_type.type != TokenType::IDENTIFIER && return_type.type != TokenType::KEYWORD_STRING && return_type.type != TokenType::KEYWORD_NUM && return_type.type != TokenType::KEYWORD_BOOL) {
        throw RuntimeError("Syntax error: expected return type");
    }

    Token name = advance();
    if (name.type != TokenType::IDENTIFIER) {
        throw RuntimeError("Syntax error: expected function name");
    }

    Function func;
    func.return_type = return_type.value;

    while (peek().type != TokenType::KEYWORD_START && peek().type != TokenType::END_OF_FILE) {
        Token param_name = advance();
        if (param_name.type != TokenType::IDENTIFIER) {
            throw RuntimeError("Syntax error: expected parameter name");
        }
        if (advance().type != TokenType::COLON) {
            throw RuntimeError("Syntax error: expected ':' after parameter name");
        }
        Token param_type = advance();
        if (param_type.type != TokenType::IDENTIFIER && param_type.type != TokenType::KEYWORD_STRING && param_type.type != TokenType::KEYWORD_NUM && param_type.type != TokenType::KEYWORD_BOOL && param_type.type != TokenType::KEYWORD_FLOAT && param_type.type != TokenType::KEYWORD_CHAR) {
            throw RuntimeError("Syntax error: expected parameter type");
        }

        bool current_has_default_value = false;
        Value current_default_value;
        if (peek().type == TokenType::EQUAL) {
            advance(); // consume '='
            current_default_value = parseExpression();
            current_has_default_value = true;
        }
        func.parameters.push_back(Parameter(param_name.value, param_type.value, current_has_default_value, current_default_value));
    }

    if (peek().type != TokenType::KEYWORD_START) {
        throw RuntimeError("Syntax error: expected 'start' before function body");
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
    if (!functions.count(name)) {
        throw RuntimeError("Undefined function '" + name + "'");
    }

    Function func = functions[name];
    std::unordered_map<std::string, Value> named_args;

    // Parse named arguments: identifier : expression
    while (peek().type == TokenType::IDENTIFIER && peekNextToken().type == TokenType::COLON) {
        Token param_name_token = advance(); // Consume parameter name (e.g., 'a')
        advance(); // Consume COLON ':'

        Value arg_value = parseExpression(); // Parse the argument value (e.g., '3')
        named_args[param_name_token.value] = arg_value;
    }

    std::vector<Value> ordered_args;
    ordered_args.reserve(func.parameters.size()); // Pre-allocate space

    // Populate ordered_args based on function's parameter definition order
    for (const auto& param : func.parameters) {
        if (named_args.count(param.name)) {
            ordered_args.push_back(named_args[param.name]);
        } else if (param.has_default_value) { // Check for default value
            ordered_args.push_back(param.default_value);
        } else {
            throw RuntimeError("Missing argument for parameter '" + param.name + "' in function '" + name + "'");
        }
    }

    if (ordered_args.size() != func.parameters.size()) {
        // This check should ideally be covered by the loop above, but good for safety.
        throw RuntimeError("Incorrect number of arguments for function '" + name + "'");
    }

    // Create a new parser for the function body
    Parser func_parser(func.body);
    func_parser.functions = functions;

    // Set up the function's local variables
    for (size_t i = 0; i < func.parameters.size(); ++i) {
        func_parser.setVariable(func.parameters[i].name, ordered_args[i]);
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