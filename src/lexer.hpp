#pragma once
#include <string>
#include <vector>

enum class TokenType {
    SHOW,
    STRING,
    NUMBER,
    IDENTIFIER,
    KEYWORD_NUM,
    KEYWORD_STRING,
    KEYWORD_BOOL,
    KEYWORD_TRUE,
    KEYWORD_FALSE,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_WHILE,
    KEYWORD_FUN,
    KEYWORD_RETURN,
    KEYWORD_START,
    KEYWORD_END,
    COMMA,
    COLON,
    EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    LEFT_PAREN, 
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    explicit Lexer(const std::string& src);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t pos = 0;

    char peek() const;
    char peekNext() const;
    char advance();
    void skipWhitespace();
    Token readString();
    Token readIdentifierOrKeyword();
    Token readNumber();
};
