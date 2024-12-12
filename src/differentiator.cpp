#include <stdio.h>

#include "node_utils.h"
#include "simplification.h"
#include "node_defines.h"

#include "bin_exp_tree.h"

static Node* diffOper(Node* root);
static Node* diffPow(Node* root);

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

#define _DEF_CMD(name, sign, ...){  \
    case(name):{                    \
        __VA_ARGS__                 \
    }                               \
}

static Node* diffOper(Node* root){
    switch(root->value.operation_type){

        #include "code_gen.h"

        default:{
            return NULL;
        }
    }
}

#undef _DEF_CMD

static Node* diffPow(Node* root){
    Node base   = getTreeValue(root);
    Node power  = getTreeValue(root);

    if (base.type == OPER && power.type == OPER){
        return _MULT(
                     _POW(_COPY_L, _COPY_R),
                     _ADD(
                          _MULT(
                                _DIFF_R,
                                _LN(_COPY_L)
                               ),
                          _MULT(
                                _DIV(_COPY_L, _COPY_R),
                                _DIFF_L
                               )
                         )
                    );
    }

    if (base.type == OPER){
        return _MULT(
                     _NUM(power.value.number),
                     _MULT(
                           _POW(_COPY_L, _NUM(power.value.number - 1)),
                           _DIFF_L
                          )
                    );
    }

    if (power.type == OPER){
        return _MULT(
                     _POW(_COPY_L, _COPY_R),
                     _LN(_COPY_L)
                    );
    }

    return _NUM(0);
}
