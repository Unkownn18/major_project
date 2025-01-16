#include "../Header/ast.h"

Ast::Ast(std::vector<Token> tokens): tokens(tokens), pos(0), head(nullptr), track(nullptr) {}

node* Ast::syntaxTree() {
    while (pos < tokens.size()) {
        if (tokens[pos].type != TokenType::KEYWORD) {
            pos++;  // Skip non-keyword tokens
        } else {
            if (tokens[pos].input == "fetch") {
                buildFetch();
            }
            else if (tokens[pos].input == "getDiv") {
                buildOneArg();
            }
            else if (tokens[pos].input == "output") {
                buildOneArg();
            } 
            else if (tokens[pos].input == "retrieve") {
                buildTwoArg();
            } 
            else if (tokens[pos].input == "clickButton") {
                buildTwoArg();
            } 
            else if (tokens[pos].input == "getInput") {
                buildInput();
            } 
            else if (tokens[pos].type == TokenType::KEYWORD && tokens[pos].input != "to") {
                // If the token is a keyword but not "to"
                if (track == nullptr) {
                    // If track is null, assign the new node as head (root of the tree)
                    head = new node(tokens[pos].input);
                    track = head;
                } else {
                    // Link the current node to the right of the previous node
                    track->right = new node(tokens[pos].input);
                    track = track->right;
                }
                pos++;
                continue;
            }
        }
    }
    return head;
}

void Ast::buildFetch() {
    // Create the root node for "fetch"
    if (head == nullptr) {
        head = new node(tokens[pos].input); // Set head if it's not already set
        track = head;  // Start tracking at the root
    } else {
        track->right = new node(tokens[pos].input); // Link the new node to the right of the current track node
        track = track->right;  // Move track to the right
    }
    pos = pos + 2;  // Move past the 'fetch' keyword

    // Check if there are any additional arguments (i.e., 'latrel')
    if (isLatrel()) {
        track->left = new node(tokens[pos].input); // Add a left child for latrel
        pos++;
    }
}

bool Ast::isLatrel() {
    Token temp = tokens[pos];
    if (temp.type == TokenType::ATTRIBUTE || temp.type == TokenType::NUMBER || temp.type == TokenType::STRING || temp.type == TokenType::URL || temp.type == TokenType::FILE) {
        return true;
    }
    return false;
}

void Ast::buildOneArg() {
    // Create a new node for the current keyword (e.g., "getDiv" or "output")
    if (track == nullptr) {
        track = new node(tokens[pos].input);
        head = track;
    } else {
        track->right = new node(tokens[pos].input);
        track = track->right;
    }
    pos = pos + 2;  // Move past the keyword

    // Add the left child if it's a valid 'latrel'
    if (isLatrel()) {
        track->left = new node(tokens[pos].input);  // Add left child
        pos++;
    }
}

void Ast::buildTwoArg() {
    // Create a new node for the current keyword (e.g., "retrieve" or "clickButton")
    if (track == nullptr) {
        track = new node(tokens[pos].input);
        head = track;
    } else {
        track->right = new node(tokens[pos].input);
        track = track->right;
    }
    pos = pos + 2;  // Move past the keyword

    // Add the first argument (left child)
    if (isLatrel()) {
        track->left = new node(tokens[pos].input);  // Add left child
        pos++;
    }

    // Check for a comma and move past it if necessary
    if (tokens[pos].type == TokenType::COMMA) {
        pos++;
    }

    // Add the second argument (another left child)
    if (isLatrel()) {
        track->left->left = new node(tokens[pos].input);  // Add second left child
        pos++;
    }
}

void Ast::buildInput() {
    // Create a new node for the "getInput" keyword
    if (track == nullptr) {
        track = new node(tokens[pos].input);
        head = track;
    } else {
        track->right = new node(tokens[pos].input);
        track = track->right;
    }
    pos = pos + 2;  // Move past the keyword

    // Add the first argument (left child)
    if (isLatrel()) {
        track->left = new node(tokens[pos].input);  // Add left child
        pos++;
    }

    // Check for commas and process additional arguments
    node *temp=track->left;
    if(tokens[pos].type==TokenType::COMMA){
        pos++;
    }
    temp->left=new node(tokens[pos].input);
    temp=temp->left;
    pos++;
    if(tokens[pos].type==TokenType::COMMA){
        pos++;
    }
    temp->left=new node(tokens[pos].input);
    pos++;
    // Check for "to" and process it if it exists
    if (tokens[pos].input == "to") {
        pos++;  // Skip "to"
        temp=temp->left;
        temp->left=new node(tokens[pos].input);
        // track->left->left = new node(tokens[pos].input);  // Add a new left child for "to"
        // pos++;
    }
}

void Ast::delAst(node *root) {
    if (root == nullptr) {
        return;
    }
    delAst(root->left);
    delAst(root->right);
    delete root;
}
