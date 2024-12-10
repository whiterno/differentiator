#ifndef LEXER_H
#define LEXER_H

#include "bin_exp_tree.h"

const int VALUE_SIZE = 20;

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

Token* lexer(char* expression);
void printTokens(Token* tokens);
void printExp(Token* tokens);
void printToken(Token* tokens);

#endif
