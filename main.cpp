#include <iostream>
#include <fstream>
#include <string>
#include "../Header/lexer.h"
#include<sstream>
#include "../Header/parser.h"
#include "../Header/ast.h"
#include "../Header/sws.h"
void print(const std::vector<Token>& tokens) {
    for (const auto& val : tokens) {
        std::string temp;
        switch (val.type) {
            case TokenType::KEYWORD: temp = "KEYWORD"; break;
            case TokenType::URL: temp = "URL"; break;
            case TokenType::NUMBER: temp = "NUMBER"; break;
            case TokenType::STRING: temp = "STRING"; break;
            case TokenType::OPEN_PARENTHESIS: temp = "OPEN_PARENTHESIS"; break;
            case TokenType::CLOSE_PARENTHESIS: temp = "CLOSE_PARENTHESIS"; break;
            case TokenType::END_OF_FILE: temp = "END_OF_FILE"; break;
            case TokenType::COMMA: temp = "COMMA"; break;
            case TokenType::INVALID: temp = "INVALID"; break;
            case TokenType::FILE: temp = "FILE"; break;
            case TokenType::ATTRIBUTE:temp="ATTRIBUTE";break;
            default: temp = "UNKNOWN"; break;
        }
        std::cout << val.input << " " << temp << "\n";
    }
}
void print(node *temp){
    if(temp==nullptr){
            return ;
    }
    std::cout<<temp->data<<"\n";
    print(temp->left);
    print(temp->right);
}

void runOwnExtensionFile(const std::string& filename) {
    // Attempt to open the file
    std::ifstream file(filename);
    std::string content;
    if (file) {
       std::stringstream buffer;
       buffer<<file.rdbuf();
       content=buffer.str();
       file.close();
       Lexer l1=Lexer(content);
       std::vector<Token> tokens=l1.tokenize();
       //print(tokens);
       Parser p=Parser(tokens);
       if(p.parse()){
            std::cout<<"This string belongs to the language";
            Ast a=Ast(tokens);
            std::cout<<"\n";
            node *t=a.syntaxTree();
            //print(t);
            scrape(t);
            a.delAst(t);
       }
       else{
        std::cout<<"This string does not  belongs to the language";
       }
    } else {
        std::cerr << "Failed to open the file: " << filename << std::endl;
    }
    
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: run <filename.Scrape>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    // Check if the file has .ownextension
    if (filename.substr(filename.find_last_of(".") + 1) != "scrape") {
        std::cerr << "Invalid file extension! Please provide a .scrape file." << std::endl;
        return 1;
    }

    // Run the file
    runOwnExtensionFile(filename);
    return 0;
}
