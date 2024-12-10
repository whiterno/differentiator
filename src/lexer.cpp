#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "lexer.h"

#include "bin_exp_tree.h"

#define _ADD        createToken(OPERATION, (TokenValue){.operation_type = ADD})
#define _SUB        createToken(OPERATION, (TokenValue){.operation_type = SUB})
#define _MULT       createToken(OPERATION, (TokenValue){.operation_type = MULT})
#define _DIV        createToken(OPERATION, (TokenValue){.operation_type = DIV})

#define _SEP(sep)   createToken(SEPARATOR, (TokenValue){.variable = sep})

static Token*       createToken(TokenType type, TokenValue value);
static Token*       addToken(Token* last_token, Token* add_token);
static TokenType    getTokenType(char value[]);

static Operations   getOper(char value[]);
static Token*       getNum(char** expression);
static void         addOpNumVar(char** expression, Token** end);
static bool         addSeparator(char** expression, Token** end);
static Token*       getStr(char** expression);

static const char* enumToString(Operations enumerator);

//DEBUG

void printTokens(Token* tokens){
    if (tokens == NULL) return;

    switch (tokens->type)
    {
    case(OPERATION):{
        printf("OPERATION: %s \n", enumToString(tokens->value.operation_type)); break;
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

void printExp(Token* tokens){
    if (tokens == NULL) return;

    switch (tokens->type)
    {
    case(OPERATION):{
        printf(" %s ", enumToString(tokens->value.operation_type)); break;
    }
    case(VARIABLE):{
        printf(" %c ", tokens->value.variable); break;
    }
    case(NUMBER):{
        printf(" %lg ", tokens->value.number); break;
    }
    case(SEPARATOR):{
        printf(" %c ", tokens->value.variable); break;
    }
    default:
        break;
    }

    printTokens(tokens->next);
}

void printToken(Token* tokens){
    if (tokens == NULL) return;

    switch (tokens->type)
    {
    case(OPERATION):{
        printf("OPERATION: %s \n", enumToString(tokens->value.operation_type)); break;
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
}
//DEBUG

Token* lexer(char* expression){
    Token* tokens = _SEP('$');
    Token* end = tokens;

    while (*expression != '\0'){
        skipSpaces(&expression);
        if (*expression == '\0') break;

        if (addSeparator(&expression, &end)) continue;
        addOpNumVar(&expression, &end);
    }

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
    bool isNum = isdigit(**expression) || (isdigit(*(*expression + 1)) && **expression == '-');

    if (isNum)  *end = addToken(*end, getNum(expression));
    else        *end = addToken(*end, getStr(expression));
}

static Token* getNum(char** expression){
    char value[VALUE_SIZE] = {};

    for (int i = 0; i < VALUE_SIZE; i++){
        if (isdigit(**expression) || **expression == '.' || **expression == '-'){
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

static Token* getStr(char** expression){
    char value[VALUE_SIZE] = {};

    for (int i = 0; i < VALUE_SIZE; i++){
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
    if (type == OPERATION){
        return createToken(OPERATION, (TokenValue){.operation_type = getOper(value)});
    }

    return createToken(VARIABLE, (TokenValue){.variable = value[0]});
}

static Operations getOper(char value[]){
    if (strcmp(value, "+") == 0)    return ADD;
    if (strcmp(value, "-") == 0)    return SUB;
    if (strcmp(value, "*") == 0)    return MULT;
    if (strcmp(value, "/") == 0)    return DIV;
    if (strcmp(value, "^") == 0)    return POW;
    if (strcmp(value, "ln") == 0)   return LN;
    if (strcmp(value, "sin") == 0)  return SIN;
    if (strcmp(value, "cos") == 0)  return COS;
    if (strcmp(value, "tg") == 0)   return TAN;
    if (strcmp(value, "ctg") == 0)  return COT;
    if (strcmp(value, "exp") == 0)  return EXP;
    if (strcmp(value, "log") == 0)  return LOG;

    return UNKNOWN;
}

static TokenType getTokenType(char value[]){
    assert(value);

    bool isOper = strcmp(value, "+")    == 0 ||
                  strcmp(value, "-")    == 0 ||
                  strcmp(value, "*")    == 0 ||
                  strcmp(value, "/")    == 0 ||
                  strcmp(value, "^")    == 0 ||
                  strcmp(value, "ln")   == 0 ||
                  strcmp(value, "sin")  == 0 ||
                  strcmp(value, "cos")  == 0 ||
                  strcmp(value, "tg")   == 0 ||
                  strcmp(value, "ctg")  == 0 ||
                  strcmp(value, "exp")  == 0 ||
                  strcmp(value, "log")  == 0;

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

#define _DESCR(enumer) case(enumer): return #enumer
static const char* enumToString(Operations enumerator){
    switch (enumerator)
    {
    _DESCR(ADD);
    _DESCR(SUB);
    _DESCR(MULT);
    _DESCR(DIV);
    _DESCR(LN);
    _DESCR(EXP);
    _DESCR(SIN);
    _DESCR(COS);
    _DESCR(TAN);
    _DESCR(COT);
    _DESCR(POW);
    _DESCR(LOG);
    _DESCR(UNKNOWN);
    }

    return "IDK_YOU_IDIOT";
}
#undef _DESCR
