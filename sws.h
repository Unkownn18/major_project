#ifndef SWS_h
#define SWS_H
#include "ast.h"
#include<curl\curl.h>
#include<gumbo.h>
#include<cstring>
#include<string>
// extern std::string response_data;
void search_by_id(GumboNode* node, const std::string &target_id);
void print_node_data(GumboNode *node);
void search_by_class(GumboNode *node, const std::string &clas);
void retrieve();
void search_attribute(GumboNode *node,GumboTag var,const std::string a,const std::string s);
void search_h1(GumboNode *node);
void search_h2(GumboNode *node);
void search_h3(GumboNode *node);
void search_h4(GumboNode *node);
void search_h5(GumboNode *node);
void search_h6(GumboNode *node);
void search_p(GumboNode *node);
void search_title(GumboNode *node);
void parse_html(const std::string &html);
void traverse(node *root,GumboNode * &node);
void scrape(node *root);
void extract_table_data(GumboNode* node);
void textElement(GumboNode *node,GumboTag var,const std::string &ch);
void processText(GumboElement *element,const std::string &ch);
void extractDivIdsAndClasses(GumboNode* node,const std::string &s);
void extractIdandClass(GumboNode* node,const std::string &s);
size_t call_back_function(void *ptr,size_t size,size_t nmemb,void *userData);
int request(std::string url);
#endif