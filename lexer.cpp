#include "../Header/lexer.h"

Lexer::Lexer(const std::string& input) : input(input), pos(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < input.size()) {
        char current_char = input[pos];
        if (isspace(current_char)) {
            pos++;
        }
        else if (current_char == ',') {
            tokens.push_back({TokenType::COMMA, std::string(1, current_char)});
            pos++;
        }
        else if (isdigit(current_char)) {
            tokens.push_back(handleNumber());
        }
        else if (isalnum(current_char)) {
            tokens.push_back(handleKeyword());
        }
        else if (current_char == '"') {
            tokens.push_back(handleString());
        }
        else if (current_char == '(') {
            tokens.push_back({TokenType::OPEN_PARENTHESIS, std::string(1, current_char)});
            pos++;
        }
        else if (current_char == ')') {
            tokens.push_back({TokenType::CLOSE_PARENTHESIS, std::string(1, current_char)});
            pos++;
        }
        else {
            tokens.push_back({TokenType::INVALID, std::string(1, current_char)});
            pos++;
        }
    }
    tokens.push_back({TokenType::END_OF_FILE, ""});
    return tokens;
}

Token Lexer::handleKeyword() {
    std::unordered_set<std::string> Keywords{
            "fetch","getInput","output","to","retrieve", "getLinks", "getAbbr", "getAddress", "getArea", "getArticle", "getAside", "getAudio", "getB", "getBase", 
            "getBdi", "getBdo", "getBlockquote", "getBody", "getBr", "getButton", "getCanvas", "getCaption", 
            "getCite", "getCode", "getCol", "getColgroup", "getData", "getDatalist", "getDd", "getDel", "getDetails", 
            "getDfns", "getDialog", "getDiv", "getDl", "getDtm", "getEm", "getEmbed", "getFieldset", "getFigcaption", 
            "getFigure", "getFooter", "getForm", "getH1", "getH2", "getH3", "getH4", "getH5", "getH6", "getHead", 
            "getHeader", "getHr", "getHtml", "getI", "getIframe", "getImg", "getInput", "getIns", "getKbd", "getLabel", 
            "getLegend", "getLi", "getMain", "getMap", "getMark", "getMeta", "getMeter", "getNav", "getNoscript", 
            "getObject", "getOl", "getOptgroup", "getOption", "getOutput", "getP", "getPicture", "getPre", "getProgress", 
            "getQ", "getRb", "getRbc", "getRp", "getRt", "getRtc", "getS", "getSamp", "getScript", "getSection", "getSelect", 
            "getSmall", "getSource", "getSpan", "getStrong", "getStyle", "getSub", "getSup", "getTable", "getTbody", "getTd", 
            "getTemplate", "getTextarea", "getTfoot", "getTh", "getThead", "getTime", "getTitle", "getTr", "getTrack", 
            "getU", "getUl", "getVar", "getVideo", "getWbr","clickButton","getId","getClass"
        
    };

    std::string result;
    while (pos < input.size() && isalnum(input[pos])) {
        result += input[pos];
        pos++;
    }
    if(isAttribute(result)){
            return {TokenType::ATTRIBUTE,result};
    }
    if (Keywords.find(result) != Keywords.end()) {
        return {TokenType::KEYWORD, result};
    }

    return {TokenType::INVALID, result};
}
bool Lexer::isAttribute(const std::string &s){
    const std::unordered_set<std::string> Attributes{
        "id","name","label","class",
            "type","value","placeholder",
    };
    if(Attributes.find(s)!=Attributes.end()){
        return true;
    }
    return false;
}

Token Lexer::handleNumber() {
    std::string result;
    while (pos < input.size() && isdigit(input[pos])) {
        result += input[pos];
        pos++;
    }
    return {TokenType::NUMBER, result};
}

Token Lexer::handleString() {
    pos++;  // Skip the opening quote
    std::string result;
    while (pos < input.size() && input[pos] != '"') {
        result += input[pos];
        pos++;
    }
    if(input[pos]!='"'){
         pos++;
        return {TokenType::INVALID,result};
    }
      // Skip the closing quote
     pos++;
    if (isUrl(result)) {
        return {TokenType::URL, result};
    }
    if (isFile(result)) {
        return {TokenType::FILE, result};
    }
    return {TokenType::STRING, result};
}

bool Lexer::isUrl(const std::string& url) {
    std::regex url_pattern(R"((https?|ftp)://([a-zA-Z0-9.-]+)(:[0-9]+)?(/[-a-zA-Z0-9@:%_+.~#?&/=]*)?)");
    return std::regex_match(url, url_pattern);
}

bool Lexer::isFile(const std::string& filename) {
    return (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".json") ||
           (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") ||
           (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv");
}
