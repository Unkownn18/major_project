#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include<vector>
#include<iostream>
#include<string>
class Parser{
    public:
    Parser(std::vector<Token> &tokens);
    bool parse();
    private:
    bool flag;
    void parseFetch();
    void parseDiv();
    void parseInput();
    void parseRetrieve();
    void parseButton();
    void parseOutput();
    void parseHtml();
    Token  getNextToken();
    void eat();
    void E();
    std::vector<Token> tokens;
    Token currentToken;
    size_t index;
  
};

#endif