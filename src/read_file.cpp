#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "read_file.h"
#include "utils.h"

#include "bin_exp_tree.h"

static Node* prefCreateTree(char** expression, Node* parent, char value[]);
static Node* infCreateTree(char** expression, Node* parent, char value[]);

static DataType getDataType(char value[]);
static NodeValue getNodeValue(char value[], DataType data_type);
static Operations getOper(char value[]);

int readExpression(BinExpTree* tree, const char* filename, ReadMode mode){
    assert(filename);
    assert(tree);

    char* expression = NULL;
    readFile(filename, &expression);

    char character = 0;
    sscanf(expression++, " %c", &character);
    if (character != '(') return NULL;

    char value[128] = {};
    switch(mode){
        case(PREFIX):{
            tree->root = prefCreateTree(&expression, NULL, value);
            return NO_ERROR;
        }
        case(INFIX):{
            tree->root = infCreateTree(&expression, NULL, value);
            return NO_ERROR;
        }
        case(POSTFIX):{
            return NO_ERROR;
        }
    }
}

static Node* prefCreateTree(char** expression, Node* parent, char value[]){
    assert(expression);

    sscanf(++(*expression), " %s", value);

    DataType data_type  = getDataType(value);
    Node* node          = createNode(NULL, NULL, data_type, getNodeValue(value, data_type));
    node->parent = parent;
    if (data_type == OPER){
        *expression     = strchr(*expression, '(') + 1;

        node->left      = prefCreateTree(expression, node, value);
        node->right     = prefCreateTree(expression, node, value);
    }
    *expression     = strchr(*expression, ')') + 1;

    return node;
}

static Node* infCreateTree(char** expression, Node* parent, char value[]){
    assert(expression);

    Node* node = createNode(NULL, NULL, OPER, (NodeValue){.operation_type = UNKNOWN_OPERATION});

    char character = 0;
    sscanf(*expression, " %c", &character);
    if (character == '('){
        *expression = strchr(*expression, '(') + 1;
        node->left = infCreateTree(expression, node, value);
        *expression = strchr(*expression, ')') + 1;
    }

    sscanf(*expression, " %s", value);
    node->parent = parent;
    node->type = getDataType(value);
    node->value = getNodeValue(value, node->type);

    if (character == '('){
        *expression = strchr(*expression, '(') + 1;
        node->right = infCreateTree(expression, node, value);
        *expression = strchr(*expression, ')') + 1;
    }

    return node;
}

static DataType getDataType(char value[]){
    assert(value);

    if (isdigit(value[0]) || (value[0] == '-' && isdigit(value[1]))) return NUM;

    if (getOper(value) != UNKNOWN_OPERATION) return OPER;

    return VAR;
}

static NodeValue getNodeValue(char value[], DataType data_type){
    if (data_type == OPER) return (NodeValue){.operation_type = getOper(value)};
    if (data_type == VAR) return (NodeValue){.variable = value[0]};
    if (data_type == NUM) return (NodeValue){.number = atof(value)};

    return (NodeValue){.operation_type = UNKNOWN_OPERATION};
}

#define _DEF_CMD(name, sign, ...){                  \
    if (strcmp(value, sign) == 0)    return name;   \
}

static Operations getOper(char value[]){

   #include "code_gen.h"

    return UNKNOWN_OPERATION;
}

#undef _DEF_CMD
