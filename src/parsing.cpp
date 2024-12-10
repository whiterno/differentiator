#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "lexer.h"
#include "parsing.h"

#include "bin_exp_tree.h"

#define _NUM(value)         createNode(NULL, NULL, NUM, (NodeValue){.number = value})
#define _VAR(value)         createNode(NULL, NULL, VAR, (NodeValue){.variable = value})
#define _E                  createNode(NULL, NULL, VAR, (NodeValue){.variable = 'e'})

#define _ADD(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = ADD})
#define _SUB(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = SUB})
#define _DIV(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = DIV})
#define _MULT(left, right)  createNode(left, right, OPER, (NodeValue){.operation_type = MULT})
#define _POW(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = POW})
#define _LOG(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = LOG})
#define _LN(right)          createNode(_E, right, OPER, (NodeValue){.operation_type = LN})
#define _EXP(right)         createNode(_E, right, OPER, (NodeValue){.operation_type = EXP})
#define _SIN(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = SIN})
#define _COS(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = COS})
#define _TAN(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = TAN})
#define _COT(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = COT})

static Node* getE(Token** tokens);
static Node* getT1(Token** tokens);
static Node* getT2(Token** tokens);
static Node* getT3(Token** tokens);
static Node* getT4(Token** tokens);
static Node* getFunc(Token** tokens);
static Node* getN(Token** tokens);
static Node* getV(Token** tokens);

static bool isFunc(Operations operation);
static void syntaxError(Token** tokens);
static void checkEnd(Token** tokens);

Node* recursiveDescent(const char* filename){
    assert(filename);

    char* expression = NULL;
    readFile(filename, &expression);

    Token* tokens = lexer(expression);
    free(expression);

    Node* root = getE(&tokens->next);

    return root;
}

static Node* getE(Token** tokens){
    if (*tokens == NULL) return NULL;

    Node* t1 = getT1(tokens);

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == ADD){
        *tokens = (*tokens)->next;

        return _ADD(t1, getE(tokens));
    }

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == SUB){
        *tokens = (*tokens)->next;

        return _SUB(t1, getE(tokens));
    }

    return t1;
}

static Node* getT1(Token** tokens){
    checkEnd(tokens);

    Node* t2 = getT2(tokens);

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == MULT){
        *tokens = (*tokens)->next;

        return _MULT(t2, getT1(tokens));
    }

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == DIV){
        *tokens = (*tokens)->next;

        return _DIV(t2, getT1(tokens));
    }

    return t2;
}

static Node* getT2(Token** tokens){
    checkEnd(tokens);

    Node* t3 = getT3(tokens);

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == POW){
        *tokens = (*tokens)->next;

        return _POW(t3, getT2(tokens));
    }

    return t3;
}

static Node* getT3(Token** tokens){
    checkEnd(tokens);

    if ((*tokens)->type == OPERATION && isFunc((*tokens)->value.operation_type)){
        return getFunc(tokens);
    }

    return getT4(tokens);
}

static Node* getT4(Token** tokens){
    checkEnd(tokens);

    if ((*tokens)->type == SEPARATOR && (*tokens)->value.variable == '('){
        *tokens = (*tokens)->next;
        Node* exp = getE(tokens);

        if (!((*tokens)->type == SEPARATOR) || !((*tokens)->value.variable == ')')){
            printf("NO CLOSING BRACKET\n");
            syntaxError(tokens);
        }

        *tokens = (*tokens)->next;

        return exp;
    }

    if ((*tokens)->type == NUMBER){
        return getN(tokens);
    }

    if ((*tokens)->type == VARIABLE){
        return getV(tokens);
    }

    syntaxError(tokens);

    return NULL;
}

static Node* getFunc(Token** tokens){
    checkEnd(tokens);

    Operations op = (*tokens)->value.operation_type;

    if (op == LOG){
        *tokens = (*tokens)->next;
        *tokens = (*tokens)->next;
        Node* func = _LOG(getE(tokens), getE(tokens));
        *tokens = (*tokens)->next;

        return func;
    }

    if (op == SIN || op == COS || op == TAN || op == COT){
        *tokens = (*tokens)->next;
        *tokens = (*tokens)->next;
        Node* func = createNode(_NUM(-1), getE(tokens), OPER, (NodeValue){.operation_type = op});
        *tokens = (*tokens)->next;

        return func;
    }

    if (op == LN || op == EXP){
        *tokens = (*tokens)->next;
        *tokens = (*tokens)->next;
        Node* func = createNode(_E, getE(tokens), OPER, (NodeValue){.operation_type = op});
        *tokens = (*tokens)->next;

        return func;
    }

    printf("NOT A FUNCTION\n");
    syntaxError(tokens);

    return NULL;
}

static Node* getN(Token** tokens){
    checkEnd(tokens);

    Node* num = _NUM((*tokens)->value.number);
    *tokens = (*tokens)->next;

    return num;
}

static Node* getV(Token** tokens){
    checkEnd(tokens);

    Node* var = _VAR((*tokens)->value.variable);
    *tokens = (*tokens)->next;

    return var;
}

static bool isFunc(Operations operation){
    switch (operation)
    {
    case(LOG): return true;
    case(LN):  return true;
    case(EXP): return true;
    case(SIN): return true;
    case(COS): return true;
    case(TAN): return true;
    case(COT): return true;
    default:
        return false;
    }
}

static void syntaxError(Token** tokens){
    printf("SYNTAX ERROR:\n");
    printExp(*tokens);
    printf("\n");

    abort();
}

static void checkEnd(Token** tokens){
    if (*tokens == NULL){
        printf("EXPRESSION IS NOT FULL");
        syntaxError(tokens);
    }
}
