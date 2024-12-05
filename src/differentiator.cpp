#include <stdio.h>

#include "node_utils.h"

#include "bin_exp_tree.h"

#define _ADD(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = ADD})
#define _SUB(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = SUB})
#define _DIV(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = DIV})
#define _MULT(left, right)  createNode(left, right, OPER, (NodeValue){.operation_type = MULT})

#define _NUM(value)         createNode(NULL, NULL, NUM, (NodeValue){.number = value})

#define _X                  createNode(NULL, NULL, VAR, (NodeValue){.variable = 'x'})

static Node* diffOper(Node* root);

Node* diff(Node* root){
    if (root == NULL) return NULL;

    switch(root->type){
        case(OPER): return diffOper(root);
        case(VAR):  return _NUM(1);
        case(NUM):  return _NUM(0);
    }
}

#define _COPY_L copyTree(root->left)
#define _COPY_R copyTree(root->right)

#define _DIFF_L diff(root->left)
#define _DIFF_R diff(root->right)

static Node* diffOper(Node* root){
    switch(root->value.operation_type){
        case(ADD):{
            return _ADD(_DIFF_L, _DIFF_R);
        }
        case(SUB):{
            return _SUB(_DIFF_L, _DIFF_R);
        }
        case(MULT):{
            return _ADD( _MULT(_DIFF_L, _COPY_R),
                         _MULT(_COPY_L, _DIFF_R)
                       );
        }
        case(DIV):{
            return _DIV(
                         _SUB( _MULT(_DIFF_L, _COPY_R),
                               _MULT(_COPY_L, _DIFF_R)
                             ),
                         _MULT(_COPY_R, _COPY_R)
                       );
        }
        default:{
            return NULL;
        }
    }
}

#undef _COPY_L
#undef _COPY_R
#undef _DIFF_L
#undef _DIFF_R
