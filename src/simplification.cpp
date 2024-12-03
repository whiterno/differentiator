#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "simplification.h"

#include "bin_exp_tree.h"

static Node solveExpression(Node value_left, Node value_right, Operations operation);

Node getTreeValue(Node* root){
    assert(root);

    Node result = {};

    if ((root->left == NULL) && (root->type == VAR)){
        result.type = OPER;
        result.value.operation_type = UNKNOWN;

        return result;
    }
    if ((root->left == NULL) && (root->type == NUM)){
        result.type = NUM;
        result.value.number = root->value.number;

        return result;
    }

    Node value_left = getTreeValue(root->left);
    Node value_right = getTreeValue(root->right);

    return solveExpression(value_left, value_right, root->value.operation_type);
}

static Node solveExpression(Node value_left, Node value_right, Operations operation){
    if ((value_left.type == OPER) || (value_right.type == OPER)){
        return value_left;
    }

    Node result = {.type = NUM};
    switch(operation){
        case(ADD):{
            result.value.number = value_left.value.number + value_right.value.number; break;
        }
        case(SUB):{
            result.value.number = value_left.value.number - value_right.value.number; break;
        }
        case(MULT):{
            result.value.number = value_left.value.number * value_right.value.number; break;
        }
        case(DIV):{
            result.value.number = value_left.value.number / value_right.value.number; break;
        }
    }

    return result;
}
