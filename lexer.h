#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <regex>

enum class TokenType {
    ATTRIBUTE,
    KEYWORD,
    URL,
    STRING,
    NUMBER,
    FILE,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    COMMA,
    INVALID,
    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string input;
};

class Lexer {
public:
    Lexer(const std::string& input);

    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;

    Token handleKeyword();
    Token handleNumber();
    Token handleString();
    bool isUrl(const std::string& url);
    bool isFile(const std::string& filename);
    bool isAttribute(const std::string &s);
};

#endif  // LEXER_H
