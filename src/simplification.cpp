#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "simplification.h"
#include "utils.h"
#include "node_utils.h"

#include "bin_exp_tree.h"

static Node solveExpression(Node value_left, Node value_right, Operations operation);

static int simplifyNode(Node* root);
static int simplifyAddSub(Node* root);
static int simplifyMult(Node* root);
static int simplifyDiv(Node* root);
static int simplifyPow(Node* root);
static int simplifyLog(Node* root);

void simplifyTree(Node* root){
    Node* tmp_tree = NULL;
    do{
        simpleCasesCollapse(root);
        constCollapse(root);

        tmp_tree = copyTree(root);
    }while (!compareTrees(tmp_tree, root));
    nodesDtor(tmp_tree);
}

Node getTreeValue(Node* root){
    assert(root);

    Node result = {};

    if ((root->left == NULL) && (root->type == VAR)){
        result.type                 = OPER;
        result.value.operation_type = UNKNOWN;

        return result;
    }
    if ((root->left == NULL) && (root->type == NUM)){
        result.type         = NUM;
        result.value.number = root->value.number;

        return result;
    }

    Node value_left     = getTreeValue(root->left);
    Node value_right    = getTreeValue(root->right);

    return solveExpression(value_left, value_right, root->value.operation_type);
}

void constCollapse(Node* root){
    if (root == NULL) return;

    Node result = getTreeValue(root);
    if (result.type == OPER){
        constCollapse(root->left);
        constCollapse(root->right);

        return;
    }

    nodesDtor(root->left);
    nodesDtor(root->right);
    root->left          = NULL;
    root->right         = NULL;
    root->type          = NUM;
    root->value.number  = result.value.number;
}

void simpleCasesCollapse(Node* root){
    if (root == NULL || root->type == NUM || root->type == VAR) return;

    if (root->left->type == OPER)  simpleCasesCollapse(root->left);
    if (root->right->type == OPER) simpleCasesCollapse(root->right);

    if (root->left->type == VAR && root->right->type == VAR)    return;

    simplifyNode(root);
}

static int simplifyNode(Node* root){
    Operations type = root->value.operation_type;

    switch(type){
        case(ADD):  return simplifyAddSub(root);
        case(SUB):  return simplifyAddSub(root);
        case(MULT): return simplifyMult(root);
        case(DIV):  return simplifyDiv(root);
        case(POW):  return simplifyPow(root);
        case(EXP):  return simplifyPow(root);
        case(LOG):  return simplifyLog(root);
        case(LN):   return simplifyLog(root);
        default:    return UNKNOWN;
    }
}

#define _CHANGE_BONDS(root, child)      \
    Node* adr = child;                  \
    memcpy(root, child, sizeof(Node));  \
    free(adr);

#define _DTOR(root)                     \
    nodesDtor(root->left);              \
    nodesDtor(root->right);             \
    root->left = NULL;                  \
    root->right = NULL;

static int simplifyAddSub(Node* root){
    if (root->left->type == NUM){
        if (cmpf(root->left->value.number, 0)){
            free(root->left);
            _CHANGE_BONDS(root, root->right);
        }
    }
    else if (root->right->type == NUM){
        if (cmpf(root->right->value.number, 0)){
            free(root->right);
            _CHANGE_BONDS(root, root->left);
        }
    }

    return NO_ERROR;
}

static int simplifyMult(Node* root){
    if (root->left->type == NUM){
        if (cmpf(root->left->value.number, 0) == 1){
            root->type = NUM;
            root->value.number = 0;
            _DTOR(root);
        }
        else if (cmpf(root->left->value.number, 1) == 1){
            free(root->left);
            _CHANGE_BONDS(root, root->right);
        }
    }
    else if (root->right->type == NUM){
        if (cmpf(root->right->value.number, 0) == 1){
            root->type = NUM;
            root->value.number = 0;
            _DTOR(root);
        }
        else if (cmpf(root->right->value.number, 1) == 1){
            free(root->right);
            _CHANGE_BONDS(root, root->left);
        }
    }

    return NO_ERROR;
}

static int simplifyDiv(Node* root){
    if (root->left->type == NUM){
        if (cmpf(root->left->value.number, 0) == 1){
            root->type = NUM;
            root->value.number = 0;
            _DTOR(root);
        }
    }

    return NO_ERROR;
}

static int simplifyPow(Node* root){
    if (root->left->type == NUM){
        if (cmpf(root->left->value.number, 0) == 1){
            root->type = NUM;
            root->value.number = 0;
            _DTOR(root);
        }
        else if (cmpf(root->left->value.number, 1) == 1){
            root->type = NUM;
            root->value.number = 1;
            _DTOR(root);
        }
    }
    else if (root->right->type == NUM){
        if (cmpf(root->right->value.number, 0) == 1){
            root->type = NUM;
            root->value.number = 1;
            _DTOR(root);
        }
        else if (cmpf(root->right->value.number, 1) == 1){
            free(root->right);
            _CHANGE_BONDS(root, root->left);
        }
    }

    return NO_ERROR;
}

static int simplifyLog(Node* root){
     if (root->right->type == NUM){
        if (cmpf(root->right->value.number, 1) == 1){
            root->type = NUM;
            root->value.number = 0;
            _DTOR(root);
        }
    }

    return NO_ERROR;
}

#undef _DTOR
#undef _CHANGE_BONDS

#define _L_NUM value_left.value.number
#define _R_NUM value_right.value.number

static Node solveExpression(Node value_left, Node value_right, Operations operation){
    if ((value_left.type == OPER) || (value_right.type == OPER)){
        value_left.type                 = OPER;
        value_left.value.operation_type = UNKNOWN;

        return value_left;
    }

    Node result = {.type = NUM};
    switch(operation){
        case(ADD):{
            result.value.number = _L_NUM + _R_NUM; break;
        }
        case(SUB):{
            result.value.number = _L_NUM - _R_NUM; break;
        }
        case(MULT):{
            result.value.number = _L_NUM * _R_NUM; break;
        }
        case(DIV):{
            result.value.number = _L_NUM / _R_NUM; break;
        }
        case(POW):{
            result.value.number = pow(_L_NUM, _R_NUM); break;
        }
        case(LN):{
            result.value.number = log(_R_NUM); break;
        }
        case(LOG):{
            result.value.number = log(_R_NUM) / log(_L_NUM); break;
        }
        case(EXP):{
            result.value.number = exp(_R_NUM); break;
        }
        case(SIN):{
            result.value.number = sin(_R_NUM); break;
        }
        case(COS):{
            result.value.number = cos(_R_NUM); break;
        }
        case(TAN):{
            result.value.number = tan(_R_NUM); break;
        }
        case(COT):{
            result.value.number = 1 / tan(_R_NUM); break;
        }
        default: return (Node){};
    }

    return result;
}
