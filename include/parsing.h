#ifndef PARSING_H
#define PARSING_H

#include "bin_exp_tree.h"

enum TokenType{
    OPERATION,
    VARIABLE,
    NUMBER,
    SEPARATOR,
};

typedef NodeValue TokenValue;

struct Token{
    TokenType type;
    TokenValue value;

    Token* next;
};

Node* recursiveDescent(const char* filename);

#endif
