#include "../Header/sws.h"

std::string response_data;
void traverse(node *root,GumboNode* &n){
  if(root==nullptr || n==nullptr){
            return ;
    }
    if(root->data=="getH1"){
            search_h1(n);
    }
    else if(root->data=="getH2"){
            search_h2(n);
    }
    else if(root->data=="getH3"){
            search_h3(n);
    }
    else if(root->data=="getH4"){
            search_h4(n);
    }
    else if(root->data=="getH2"){
            search_h5(n);
    }
    else if(root->data=="getH5"){
            search_h5(n);
    }
    else if(root->data=="getH6"){
            search_h6(n);
    }
    else if(root->data=="getLinks"){
            search_attribute(n,GUMBO_TAG_LINK,"href","Link");
    }
    else if(root->data=="getP"){
            search_p(n);
    }
    else if(root->data=="getAbbr"){
            search_attribute(n,GUMBO_TAG_ABBR,"title","");
    }
    else if(root->data=="retrieve"){
        if(root->left->data=="id"){
                search_by_id(n,root->left->left->data);
        }
        else{
            search_by_class(n,root->left->left->data);
        }
    }
    else if(root->data=="getTitle"){
            search_title(n);
    }
    else if(root->data=="getData"){
            std::cout<<response_data;
    }
    else if(root->data=="getTable"){
            extract_table_data(n);
    }
    else if(root->data=="getDiv"){
         extractDivIdsAndClasses(n,root->left->data);
    }
    else if(root->data=="getId"){
            extractIdandClass(n,"id");
    }
    else if(root->data=="getClass"){
            extractIdandClass(n,"class");
    }
    else if(root->data=="getImg"){
            search_attribute(n,GUMBO_TAG_IMG,"src","Image");
    }
    else if(root->data=="getVideo"){
            search_attribute(n,GUMBO_TAG_A,"src","Video Url ");
            search_attribute(n,GUMBO_TAG_IFRAME,"src","Video Url");
    }
    traverse(root->right,(GumboNode *&)n);
}



void scrape(node *root){
    std::string url=root->data;
    int res=request(root->left->data);
    if(res==0){
    GumboOutput *output=gumbo_parse(response_data.c_str());
    if(output){
       traverse(root->right,(GumboNode *&)(output->root));
    }
    else{
        std::cerr<<"Error Parsing Html\n";
    }
    gumbo_destroy_output(&kGumboDefaultOptions,output);
    }
}



int request(std::string url){
    CURL *curl;
    CURLcode res;
    curl=curl_easy_init();
    if(curl){
        curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,call_back_function);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&response_data);
        res=curl_easy_perform(curl);
        if(res!=CURLE_OK){
            std::cerr << "CURL error: " << curl_easy_strerror(res) << " while fetching: " << url << std::endl;
            return -1;
        }
    }
    else{
        std::cerr<<"Curl Initialization failed for URL\n";
        return -1;
    }
    curl_easy_cleanup(curl);
    return 0;
}


size_t call_back_function(void *ptr,size_t size,size_t nmemb,void *userData){
size_t total_size=size*nmemb;
std::string *response_str=static_cast<std::string *>(userData);
response_str->append(static_cast<char *>(ptr),total_size);
return total_size;

}
   
void search_attribute(GumboNode *node,GumboTag var,const std::string a,const std::string s){
    if(node->type!=GUMBO_NODE_ELEMENT){
        return ;
    }
    GumboElement *element=&node->v.element;
    if(element->tag==var){
        for(size_t i=0;i<element->attributes.length;++i){
            GumboAttribute *attr=(GumboAttribute *)(element->attributes.data[i]);
            if(attr && strcmp(attr->name,a.c_str())==0){
                    std::cout<<s<<" "<<attr->value<<"\n";
            }
        }
    }
    for(size_t i=0;i<element->children.length;++i){
        search_attribute((GumboNode *)element->children.data[i],var,a,s);
    }

}


void search_h1(GumboNode *node) {
    textElement(node, GUMBO_TAG_H1, "H1");
}
void search_h2(GumboNode *node) {
    textElement(node, GUMBO_TAG_H2, "H2");
}
void search_h3(GumboNode *node) {
    textElement(node, GUMBO_TAG_H3, "H3");
}
void search_h4(GumboNode *node) {
    textElement(node, GUMBO_TAG_H4, "H4");
}
void search_h5(GumboNode *node) {
    textElement(node, GUMBO_TAG_H5, "H5");
}
void search_h6(GumboNode *node) {
    textElement(node, GUMBO_TAG_H6, "H6");
}
void search_p(GumboNode *node) {
    textElement(node, GUMBO_TAG_P, "P");
}
void search_title(GumboNode *node){
        textElement(node,GUMBO_TAG_TITLE,"");
}

void processText(GumboElement *element, const std::string &ch) {
    for (size_t i = 0; i < element->children.length; ++i) {
        GumboNode *child = (GumboNode *)element->children.data[i];
        if (child->type == GUMBO_NODE_TEXT) {
            std::cout << ch << " " << child->v.text.text << "\n";
        }
    }
}

void textElement(GumboNode *node, GumboTag tag, const std::string &ch) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboElement *element = &node->v.element;

    if (element->tag == tag) {
        processText(element, ch);
        
    }
    for (size_t i = 0; i < element->children.length; ++i) {
        GumboNode *child_node = (GumboNode *)element->children.data[i];
        textElement(child_node, tag, ch);
    }
}



void search_by_id(GumboNode *node, const std::string &id) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    // Loop through the attributes to check if 'id' exists
    GumboAttribute *attr = NULL;
    for (size_t i = 0; i < node->v.element.attributes.length; i++) {
        attr = (GumboAttribute *)node->v.element.attributes.data[i];

        // Check if the attribute is 'id' and if it matches the desired id
        if (attr && strcmp(attr->name, "id") == 0 && strcmp(attr->value, id.c_str()) == 0) {
            std::cout<<"Found element with id "<<id<<"\n";
            print_node_data(node);
            // Process the content of the element as needed
        }
    }

    // Recursively search all children of this node
    for (size_t i = 0; i < node->v.element.children.length; i++) {
        // Cast the void pointer to GumboNode* before passing it to search_by_id
        search_by_id((GumboNode*)node->v.element.children.data[i], id);
    }
}


void search_by_class(GumboNode *node, const std::string &clas) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    // Loop through the attributes to check if 'id' exists
    GumboAttribute *attr = NULL;
    for (size_t i = 0; i < node->v.element.attributes.length; i++) {
        attr = (GumboAttribute *)node->v.element.attributes.data[i];

        // Check if the attribute is 'id' and if it matches the desired id
        if (attr && strcmp(attr->name, "class") == 0 && strcmp(attr->value, clas.c_str()) == 0) {
            std::cout<<"Found element with class "<<clas<<"\n";
            print_node_data(node);
            // Process the content of the element as needed
        }
    }

    // Recursively search all children of this node
    for (size_t i = 0; i < node->v.element.children.length; i++) {
        // Cast the void pointer to GumboNode* before passing it to search_by_id
        search_by_class((GumboNode*)node->v.element.children.data[i], clas);
    }
}


void print_node_data(GumboNode *node) {
    // Ensure that the node is an element node and has text content
    if (node->type == GUMBO_NODE_TEXT) {
        std::cout << node->v.text.text << std::endl;
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        // Loop through the children of the element to recursively print all text content
        for (size_t i = 0; i < node->v.element.children.length; ++i) {
            print_node_data((GumboNode*)node->v.element.children.data[i]);
        }
    }
}


void extract_table_data(GumboNode* node) {
    if (node == nullptr) return;

    // If the current node is an element node, check its tag
    if (node->type == GUMBO_NODE_ELEMENT) {
        GumboElement* element = &node->v.element;

        // Check if the element is a <table> element
        if (element->tag == GUMBO_TAG_TABLE) {
            std::cout << "Found <table> element!" << std::endl;

            // Process the <table> element (same logic as before)
            for (size_t i = 0; i < element->children.length; ++i) {
                GumboNode* child = static_cast<GumboNode*>(element->children.data[i]);

                // Look for <tr> elements (table rows)
                if (child->type == GUMBO_NODE_ELEMENT) {
                    GumboElement* tr_element = &child->v.element;
                    if (tr_element->tag == GUMBO_TAG_TR) {
                        std::cout << "| ";

                        for (size_t j = 0; j < tr_element->children.length; ++j) {
                            GumboNode* td = static_cast<GumboNode*>(tr_element->children.data[j]);

                            if (td->type == GUMBO_NODE_ELEMENT) {
                                GumboElement* td_element = &td->v.element;

                                if (td_element->tag == GUMBO_TAG_TD || td_element->tag == GUMBO_TAG_TH) {
                                    bool has_text = false;

                                    // Find the text inside <td> or <th> elements
                                    for (size_t k = 0; k < td_element->children.length; ++k) {
                                        GumboNode* text_node = static_cast<GumboNode*>(td_element->children.data[k]);

                                        if (text_node->type == GUMBO_NODE_TEXT) {
                                            GumboText* text = &text_node->v.text;
                                            std::cout << text->text << " | ";
                                            has_text = true;
                                        }
                                    }

                                    // If no text was found, print empty
                                    if (!has_text) {
                                        std::cout << "Empty | ";
                                    }
                                }
                            }
                        }
                        std::cout << std::endl;  // End of row
                    }
                }
            }
        }

        // Recursively call the function on each child node
        for (size_t i = 0; i < element->children.length; ++i) {
            GumboNode* child = static_cast<GumboNode*>(element->children.data[i]);
            extract_table_data(child);  // Recursive call to check child nodes
        }
    }
}




void extractDivIdsAndClasses(GumboNode* node,const std::string &s) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    GumboElement* element = &node->v.element;

    // Check if this element is a div
    if (element->tag == GUMBO_TAG_DIV) {
        // Check for 'id' attribute
        for (size_t i = 0; i < element->attributes.length; ++i) {
            GumboAttribute* attr = static_cast<GumboAttribute*>(element->attributes.data[i]);
            if(attr && strcmp(attr->name,s.c_str())==0){
                    std::cout<<s<<" "<<attr->value<<"\n";
            }
        }
    }

    // Traverse child nodes recursively
    for (size_t i = 0; i < element->children.length; ++i) {
        extractDivIdsAndClasses(static_cast<GumboNode*>(element->children.data[i]),s);
    }
}

void extractIdandClass(GumboNode* node,const std::string &s){
     if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    GumboElement* element = &node->v.element;
     for (size_t i = 0; i < element->attributes.length; ++i) {
            GumboAttribute* attr = static_cast<GumboAttribute*>(element->attributes.data[i]);
            if(attr && strcmp(attr->name,s.c_str())==0){
                    std::cout<<s<<" "<<attr->value<<"\n";
            }
        }
    

    // Traverse child nodes recursively
    for (size_t i = 0; i < element->children.length; ++i) {
        extractIdandClass(static_cast<GumboNode*>(element->children.data[i]),s);
    }
    

}