#ifndef AST_H
#define AST_H
#include "lexer.h"
#include<string>
#include<vector>
typedef struct node{
std::string data;
node *left;
node *right;
node(std::string d){
        data=d;
        left=nullptr;
        right=nullptr;
}
}node;

class Ast{
public:
Ast(std::vector<Token> tokens);
node *syntaxTree();
void delAst(node *root);
private:
size_t pos;
node *head;
node *track;
std::vector<Token> tokens;
void buildFetch();
void buildOneArg();
void buildTwoArg();
void buildInput();
bool isLatrel();
};

#endif