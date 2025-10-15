#include "lexer.hpp"
#include <cctype>

Lexer::Lexer(const std::string& src) : source(src) {}

char Lexer::peek() const {
    if (pos >= source.size()) return '\0';
    return source[pos];
}

char Lexer::advance() {
    if (pos < source.size()) return source[pos++];
    return '\0';
}

char Lexer::peekNext() const {
    if (pos + 1 >= source.size()) return '\0';
    return source[pos + 1];
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek())) advance();
}

void Lexer::skipComment() {
    while (peek() != '\n' && peek() != '\0') {
        advance();
    }
    if (peek() == '\n') {
        advance(); // consume the newline
    }
}

Token Lexer::readString() {
    advance(); // skip opening quote
    std::string value;
    while (peek() != '"' && peek() != '\0') {
        value += advance();
    }
    advance(); // skip closing quote
    return { TokenType::STRING, value };
}

Token Lexer::readIdentifierOrKeyword() {
    std::string value;
    while (std::isalnum(peek()) || peek() == '_') {
        value += advance();
    }
    if (value == "show")
        return { TokenType::SHOW, value };
    if (value == "num")
        return { TokenType::KEYWORD_NUM, value };
    if (value == "string")
        return { TokenType::KEYWORD_STRING, value };
    if (value == "bool")
        return { TokenType::KEYWORD_BOOL, value };
    if (value == "float")
        return { TokenType::KEYWORD_FLOAT, value };
    if (value == "char")
        return { TokenType::KEYWORD_CHAR, value };
    if (value == "true")
        return { TokenType::KEYWORD_TRUE, value };
    if (value == "false")
        return { TokenType::KEYWORD_FALSE, value };
    if (value == "if")
        return { TokenType::KEYWORD_IF, value };
    if (value == "else")
        return { TokenType::KEYWORD_ELSE, value };
    if (value == "while")
        return { TokenType::KEYWORD_WHILE, value };
    if (value == "fun")
        return { TokenType::KEYWORD_FUN, value };
    if (value == "return")
        return { TokenType::KEYWORD_RETURN, value };
    if (value == "start")
        return { TokenType::KEYWORD_START, value };
    if (value == "end")
        return { TokenType::KEYWORD_END, value };
    return { TokenType::IDENTIFIER, value };
}

Token Lexer::readNumber() {
    std::string value;
    while (std::isdigit(peek())) {
        value += advance();
    }
    if (peek() == '.') {
        value += advance(); // consume '.'
        while (std::isdigit(peek())) {
            value += advance();
        }
    }
    return { TokenType::NUMBER, value };
}

Token Lexer::readChar() {
    advance(); // skip opening single quote
    char c = advance(); // read the character
    if (peek() != '\'') {
        // Error: expected closing single quote
        return { TokenType::UNKNOWN, "" };
    }
    advance(); // skip closing single quote
    return { TokenType::CHAR, std::string(1, c) };
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (peek() != '\0') {
        skipWhitespace();
        char c = peek();

        if (c == '"')
            tokens.push_back(readString());
        else if (std::isalpha(c))
            tokens.push_back(readIdentifierOrKeyword());
        else if (std::isdigit(c))
            tokens.push_back(readNumber());
        else if (c == '\'')
            tokens.push_back(readChar());
        else if (c == '=') {
            if (peekNext() == '=') {
                advance(); advance();
                tokens.push_back({ TokenType::EQUAL_EQUAL, "==" });
            } else {
                advance();
                tokens.push_back({ TokenType::EQUAL, "=" });
            }
        }
        else if (c == '+') {
            advance();
            tokens.push_back({ TokenType::PLUS, "+" });
        }
        else if (c == '-') {
            advance();
            tokens.push_back({ TokenType::MINUS, "-" });
        }
        else if (c == '*') {
            advance();
            tokens.push_back({ TokenType::STAR, "*" });
        }
        else if (c == '/') {
            if (peekNext() == '/') {
                skipComment();
                continue; // Continue to the next token after skipping comment
            } else {
                advance();
                tokens.push_back({ TokenType::SLASH, "/" });
            }
        }
        else if (c == '<') {
            if (peekNext() == '=') {
                advance(); advance();
                tokens.push_back({ TokenType::LESS_EQUAL, "<=" });
            } else {
                advance();
                tokens.push_back({ TokenType::LESS, "<" });
            }
        }
        else if (c == '>') {
            if (peekNext() == '=') {
                advance(); advance();
                tokens.push_back({ TokenType::GREATER_EQUAL, ">=" });
            } else {
                advance();
                tokens.push_back({ TokenType::GREATER, ">" });
            }
        }
        else if (c == '!') {
            if (peekNext() == '=') {
                advance(); advance();
                tokens.push_back({ TokenType::NOT_EQUAL, "!=" });
            } else {
                advance();
                tokens.push_back({ TokenType::UNKNOWN, "!" });
            }
        }
        else if (c == '(') {
            advance();
            tokens.push_back({ TokenType::LEFT_PAREN, "(" });
        }
        else if (c == ')') {
            advance();
            tokens.push_back({ TokenType::RIGHT_PAREN, ")" });
        }
        else if (c == ',') {
            advance();
            tokens.push_back({ TokenType::COMMA, "," });
        }
        else if (c == ':') {
            advance();
            tokens.push_back({ TokenType::COLON, ":" });
        }
        else if (c == '\0')
            break;
        else {
            advance();
            tokens.push_back({ TokenType::UNKNOWN, std::string(1, c) });
        }
    }

    tokens.push_back({ TokenType::END_OF_FILE, "" });
    return tokens;
}
