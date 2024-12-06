#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "parsing.h"

#include "bin_exp_tree.h"

#define _ADD        createToken(OPERATION, (TokenValue){.operation_type = ADD})
#define _SUB        createToken(OPERATION, (TokenValue){.operation_type = SUB})
#define _MULT       createToken(OPERATION, (TokenValue){.operation_type = MULT})
#define _DIV        createToken(OPERATION, (TokenValue){.operation_type = DIV})

#define _SEP(sep)   createToken(SEPARATOR, (TokenValue){.variable = sep})

static Token* lexer(char* expression);

static Token*       createToken(TokenType type, TokenValue value);
static Token*       addToken(Token* last_token, Token* add_token);
static TokenType    getTokenType(char value[]);

static Operations   getOper(char value[]);
static Token*       getNum(char** expression);
static void         addOpNumVar(char** expression, Token** end);
static bool         addSeparator(char** expression, Token** end);
static Token*       getLexem(char** expression);

static void printTokens(Token* tokens);

Node* recursiveDescent(const char* filename){
    assert(filename);

    char* expression = NULL;
    readFile(filename, &expression);

    printf("FILE: %s\n Check \n", expression);

    Token* tokens = lexer(expression);
    printTokens(tokens);

    return NULL;
}

//DEBUG

static void printTokens(Token* tokens){
    if (tokens == NULL) return;

    switch (tokens->type)
    {
    case(OPERATION):{
        printf("OPERATION: %d \n", tokens->value.operation_type); break;
    }
    case(VARIABLE):{
        printf("VARIABLE:  %c \n", tokens->value.variable); break;
    }
    case(NUMBER):{
        printf("NUMBER:    %lg \n", tokens->value.number); break;
    }
    case(SEPARATOR):{
        printf("SEPARATOR: %c \n", tokens->value.variable); break;
    }
    default:
        break;
    }

    printTokens(tokens->next);
}

//DEBUG

static Token* lexer(char* expression){
    Token* tokens = _SEP('$');
    Token* end = tokens;

    while (*expression != '\0'){
        skipSpaces(&expression);
        if (*expression == '\0') break;

        if (addSeparator(&expression, &end)) continue;
        addOpNumVar(&expression, &end);
    }

    end->next = _SEP('$');

    return tokens;
}

static bool addSeparator(char** expression, Token** end){
    if (**expression == '('){
        *end = addToken(*end, _SEP('('));
        (*expression)++;

        return true;
    }
    if (**expression == ')'){
        *end = addToken(*end, _SEP(')'));
        (*expression)++;

        return true;
    }

    return false;
}

static void addOpNumVar(char** expression, Token** end){
    if (isdigit(**expression))  *end = addToken(*end, getNum(expression));
    else                        *end = addToken(*end, getLexem(expression));
}

static Token* getNum(char** expression){
    char value[20] = {};

    for (int i = 0; i < 20; i++){
        if (isdigit(**expression) || **expression == '.'){
            value[i] = **expression;
            (*expression)++;
        }
        else{
            value[i] = '\0';
            break;
        }
    }

    return createToken(NUMBER, (TokenValue){.number = atof(value)});
}

static Token* getLexem(char** expression){
    char value[20] = {};

    for (int i = 0; i < 20; i++){
        if (**expression != '(' && **expression != ')' && **expression != ' ' &&
           (**expression <= '0' || ** expression >= '9')){
            value[i] = **expression;
            (*expression)++;
        }
        else{
            value[i] = '\0';
            break;
        }
    }

    TokenType type = getTokenType(value);
    if (type == OPERATION) return createToken(OPERATION, (TokenValue){.operation_type = getOper(value)});

    return createToken(VARIABLE, (TokenValue){.variable = value[0]});
}

static Operations getOper(char value[]){
    if (strcmp(value, "+") == 0) return ADD;
    if (strcmp(value, "-") == 0) return SUB;
    if (strcmp(value, "*") == 0) return MULT;
    if (strcmp(value, "/") == 0) return DIV;

    return UNKNOWN;
}

static TokenType getTokenType(char value[]){
    assert(value);

    bool isOper = strcmp(value, "+") == 0 ||
                  strcmp(value, "-") == 0 ||
                  strcmp(value, "*") == 0 ||
                  strcmp(value, "/") == 0;

    if (isOper) return OPERATION;

    return VARIABLE;
}

static Token* createToken(TokenType type, TokenValue value){
    Token* token = (Token*)calloc(1, sizeof(Token));

    token->type = type;
    token->next = NULL;
    memcpy(&token->value, &value, sizeof(TokenValue));

    return token;
}

static Token* addToken(Token* last_token, Token* add_token){
    if (last_token == NULL){
        return add_token;
    }

    last_token->next = add_token;

    return last_token->next;
}
