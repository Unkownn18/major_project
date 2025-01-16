#include "../Header/parser.h"
int line=1;
Parser::Parser(std::vector<Token> &tokens):tokens(tokens),index(0),flag(true){}
void Parser::eat(){
    if(index>=tokens.size()){
                return ;
    }
    index++;
}
bool Parser::parse(){
    if(tokens.empty()){
        std::cout<<"error! Program is Empty\n";
            return false;
    }
    if(tokens[0].input!="fetch"){
        std::cout<<"error! Missing Fetch at "<<tokens[0].input<<"?? ,line "<<line<<"\n";
        return false;
    }
    E();
    return flag;

}
Token Parser::getNextToken(){
    if(index>=tokens.size()){
        return {TokenType::END_OF_FILE,""};
    }
    return tokens[index];
}

void Parser::E(){
    if(getNextToken().type==TokenType::END_OF_FILE || (!flag)){
            return ;
    }
    if(getNextToken().input=="fetch"){
            parseFetch();
    }
    else if(getNextToken().input=="getDiv"){
            parseDiv();
    }
    else if(getNextToken().input=="retrieve"){
            parseRetrieve();
    }
    else if(getNextToken().input=="getInput"){
            parseInput();
    }
    else if(getNextToken().input=="clickButton"){
            parseButton();
    }
    else if(getNextToken().input=="output"){
            parseOutput();
    }
    else if(getNextToken().type==TokenType::KEYWORD){
            parseHtml();
    }
    else{
        flag=false;
        return ;
    }
    line++;
    E();
}

void Parser::parseFetch(){
    if(index!=0){
        std::cout<<"error! Redecleration of fetch is not allowed ,line "<<line<<"\n";
       flag=false;
        return ;
    }
    eat();
    if(getNextToken().type==TokenType::OPEN_PARENTHESIS){
                eat();
            if(getNextToken().type==TokenType::URL){
                               eat();
                    if(getNextToken().type==TokenType::CLOSE_PARENTHESIS){
                                           eat();
                    }
                    else{
                        std::cout<<"error! missing ')' for fetch( "<<tokens[index-1].input<<"  ,line "<<line<<"\n";
                        flag=false;
                    }
            }
            else{
                std::cout<<"error! missing URL for fetch( , line "<<line<<"\n";
                flag=false;
            }
    }
    else{
        std::cout<<"error! missing '(' after fetch , line "<<line<<"\n";
        flag=false;
    }
}

void Parser::parseDiv(){
        eat();
        if(getNextToken().type==TokenType::OPEN_PARENTHESIS){
                eat();
                if(getNextToken().input=="id" || getNextToken().input=="class"){
                            eat();
                            if(getNextToken().type==TokenType::CLOSE_PARENTHESIS){
                                    eat();
                            }
                            else{
                                    std::cout<<"error! missing ')' for getDiv( "<<tokens[index-1].input<<" , line "<<line<<"\n";
                                    flag=false;
                            }
                }
                else{
                    std::cout<<"error! class and id is missing for getDiv() , line "<<line<<"\n";
                    flag=false;
                }
        }
        else{
            std::cout<<"error! missing '(' after getDiv , line "<<line<<"\n";
            flag=false;
        }
}

void Parser::parseRetrieve(){
    eat();
    if(getNextToken().type==TokenType::OPEN_PARENTHESIS){
            eat();
            if(getNextToken().type==TokenType::ATTRIBUTE){
                        eat();
                        if(getNextToken().type==TokenType::COMMA){
                                eat();
                                if(getNextToken().type==TokenType::STRING){
                                        eat();
                                        if(getNextToken().type==TokenType::CLOSE_PARENTHESIS){
                                                eat();
                                        }
                                        else{
                                            std::cout<<"error! missing a  ')' for retrieve , line "<<line<<"\n";
                                            flag=false;
                                        }
                                }
                                else{
                                    std::cout<<"error! missing attributes value retrieve( , line "<<line<<"\n";
                                    flag=false;
                                }
                        }
                        else{
                            std::cout<<"error! missing a comma ',' after retrieve( "<<tokens[index-1].input<<"  , line "<<line<<"\n";
                            flag=false;
                        }
            }
            else{
                std::cout<<"error! missing id and class for retrieve(  , line "<<line<<"\n";
                flag=false;
            }
    }
    else{
        std::cout<<"error! missing a '(' after retrieve , line "<<line<<"\n";
        flag=false;
    }
}

void Parser::parseInput(){
    eat();
    if(getNextToken().type==TokenType::OPEN_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing a '(' for getInput , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::ATTRIBUTE){
            eat();
    }
    else{
        std::cout<<"error! missing a attribute after getInput(  , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::COMMA){
        eat();
    }
    else{
        std::cout<<"error! missing a comma ',' after attribute for getInput , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::STRING){
            eat();
    }
    else{
        std::cout<<"error! missing attribute value for getInput , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::COMMA){
        eat();
    }
    else{
        std::cout<<"error! missing a comma ',' after attribute for getInput , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::STRING || getNextToken().type==TokenType::NUMBER){
        eat();
    }
    else{
        std::cout<<"error! missing input value for getInput  , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().input=="to"){
        eat();
        if(getNextToken().type==tokens[index-2].type){
                eat();
        }
        else{
            std::cout<<"error! input values are not same , line "<<line<<"\n";
            flag=false;
            return ;   
        }
        
    }
    if(getNextToken().type==TokenType::CLOSE_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing ')' for getInput  , line "<<line<<"\n";
        flag=false;
        return ;
    }
}

void Parser::parseButton(){
    eat();
    if(getNextToken().type==TokenType::OPEN_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing a '(' for clickButton  , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::ATTRIBUTE){
            eat();
    }
    else{
        std::cout<<"error! missing a attribute after clickButton(  , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::COMMA){
        eat();
    }
    else{
        std::cout<<"error! missing a comma ',' after attribute for clickButton , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::STRING){
            eat();
    }
    else{
        std::cout<<"error! missing attribute value for clickButton , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::CLOSE_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing ')' for clickButton  , line "<<line<<"\n";
        flag=false;
        return ;
    }
}

void Parser::parseOutput(){
    eat();
    if(getNextToken().type==TokenType::OPEN_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing a '(' for output  , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::FILE){
                eat();
    }
    else{
        std::cout<<"error! missing file name for output , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::CLOSE_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing ')' for output  , line "<<line<<"\n";
        flag=false;
        return ;
    }
    
}

void Parser::parseHtml(){
    std::string temp=getNextToken().input;
    eat();
    if(getNextToken().type==TokenType::OPEN_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing a '(' for "<<temp<<" , line "<<line<<"\n";
        flag=false;
        return ;
    }
    if(getNextToken().type==TokenType::CLOSE_PARENTHESIS){
            eat();
    }
    else{
        std::cout<<"error! missing ')' for "<<temp<<"( , line "<<line<<"\n";
        flag=false;
        return ;
    }
}