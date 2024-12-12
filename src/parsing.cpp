#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "lexer.h"
#include "parsing.h"
#include "node_utils.h"
#include "node_defines.h"

#include "bin_exp_tree.h"

static Node* getExpression(Token** tokens);
static Node* getMultDiv(Token** tokens);
static Node* getPow(Token** tokens);
static Node* getFuncNumVar(Token** tokens);
static Node* getNumVar(Token** tokens);
static Node* getFunc(Token** tokens);
static Node* getNum(Token** tokens);
static Node* getVar(Token** tokens);

static bool isFunc(Operations operation);
static void syntaxError(Token** tokens);
static void checkEnd(Token** tokens);

Node* recursiveDescent(const char* filename){
    assert(filename);

    char* expression = NULL;
    readFile(filename, &expression);

    Token* tokens = lexer(expression);
    free(expression);

    Node* root = getExpression(&tokens->next);
    makeParents(root, NULL);

    return root;
}

static Node* getExpression(Token** tokens){
    if (*tokens == NULL) return NULL;

    Node* t1 = getMultDiv(tokens);

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == ADD){
        *tokens = (*tokens)->next;

        return _ADD(t1, getExpression(tokens));
    }

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == SUB){
        *tokens = (*tokens)->next;

        return _SUB(t1, getExpression(tokens));
    }

    return t1;
}

static Node* getMultDiv(Token** tokens){
    checkEnd(tokens);

    Node* t2 = getPow(tokens);

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == MULT){
        *tokens = (*tokens)->next;

        return _MULT(t2, getMultDiv(tokens));
    }

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == DIV){
        *tokens = (*tokens)->next;

        return _DIV(t2, getMultDiv(tokens));
    }

    return t2;
}

static Node* getPow(Token** tokens){
    checkEnd(tokens);

    Node* t3 = getFuncNumVar(tokens);

    if ((*tokens != NULL) && (*tokens)->type == OPERATION && (*tokens)->value.operation_type == POW){
        *tokens = (*tokens)->next;

        return _POW(t3, getPow(tokens));
    }

    return t3;
}

static Node* getFuncNumVar(Token** tokens){
    checkEnd(tokens);

    if ((*tokens)->type == OPERATION && isFunc((*tokens)->value.operation_type)){
        return getFunc(tokens);
    }

    return getNumVar(tokens);
}

static Node* getNumVar(Token** tokens){
    checkEnd(tokens);

    if ((*tokens)->type == SEPARATOR && (*tokens)->value.variable == '('){
        *tokens = (*tokens)->next;
        Node* exp = getExpression(tokens);

        if (!((*tokens)->type == SEPARATOR) || !((*tokens)->value.variable == ')')){
            printf("NO CLOSING BRACKET\n");
            syntaxError(tokens);
        }

        *tokens = (*tokens)->next;

        return exp;
    }

    if ((*tokens)->type == NUMBER){
        return getNum(tokens);
    }

    if ((*tokens)->type == VARIABLE){
        return getVar(tokens);
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
        Node* func = _LOG(getExpression(tokens), getExpression(tokens));
        *tokens = (*tokens)->next;

        return func;
    }

    if (op == SIN || op == COS || op == TAN || op == COT){
        *tokens = (*tokens)->next;
        *tokens = (*tokens)->next;
        Node* func = createNode(_NUM(-1), getExpression(tokens), OPER, (NodeValue){.operation_type = op});
        *tokens = (*tokens)->next;

        return func;
    }

    if (op == LN || op == EXP){
        *tokens = (*tokens)->next;
        *tokens = (*tokens)->next;
        Node* func = createNode(_E, getExpression(tokens), OPER, (NodeValue){.operation_type = op});
        *tokens = (*tokens)->next;

        return func;
    }

    printf("NOT A FUNCTION\n");
    syntaxError(tokens);

    return NULL;
}

static Node* getNum(Token** tokens){
    checkEnd(tokens);

    Node* num = _NUM((*tokens)->value.number);
    *tokens = (*tokens)->next;

    return num;
}

static Node* getVar(Token** tokens){
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
