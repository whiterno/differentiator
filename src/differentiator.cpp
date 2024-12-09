#include <stdio.h>

#include "node_utils.h"
#include "simplification.h"

#include "bin_exp_tree.h"

#define _NUM(value)         createNode(NULL, NULL, NUM, (NodeValue){.number = value})

#define _ADD(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = ADD})
#define _SUB(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = SUB})
#define _DIV(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = DIV})
#define _MULT(left, right)  createNode(left, right, OPER, (NodeValue){.operation_type = MULT})
#define _POW(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = POW})
#define _LN(right)          createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = LN})
#define _EXP(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = EXP})
#define _SIN(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = SIN})
#define _COS(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = COS})
#define _TAN(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = TAN})
#define _COT(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = COT})


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
        case(POW):{
            return diffPow(root);
        }
        case(EXP):{
            return _MULT(
                         _EXP(_COPY_R),
                         _DIFF_R
                        );
        }
        case(LN):{
            return _MULT(
                         _DIV(_NUM(1), _COPY_R),
                         _DIFF_R
                        );
        }
        case(LOG):{
            return diff(_DIV(
                             _LN(_COPY_R),
                             _LN(_COPY_L)
                            ));
        }
        case(SIN):{
            return _MULT(
                         _COS(_COPY_R),
                         _DIFF_R
                        );
        }
        case(COS):{
            return _MULT(_NUM(-1),
                         _MULT(
                               _SIN(_COPY_R),
                               _DIFF_R
                              )
                        );
        }
        case(TAN):{
            return _MULT(
                         _DIV(
                              _NUM(1),
                              _POW(_COS(_COPY_R), _NUM(2))
                             ),
                         _DIFF_R
                        );
        }
        case(COT):{
            return _MULT(
                         _DIV(
                              _NUM(1),
                              _POW(_SIN(_COPY_R), _NUM(2))
                             ),
                         _MULT(_NUM(-1), _DIFF_R)
                        );
        }
        default:{
            return NULL;
        }
    }
}

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
