#ifndef NODE_DEFINES_H
#define NODE_DEFINES_H

#define _NUM(value)         createNode(NULL, NULL, NUM, (NodeValue){.number = value})
#define _VAR(value)         createNode(NULL, NULL, VAR, (NodeValue){.variable = value})
#define _E                  _VAR('e')

#define _ADD(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = ADD})
#define _SUB(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = SUB})
#define _DIV(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = DIV})
#define _MULT(left, right)  createNode(left, right, OPER, (NodeValue){.operation_type = MULT})
#define _POW(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = POW})
#define _LOG(left, right)   createNode(left, right, OPER, (NodeValue){.operation_type = LOG})
#define _LN(right)          createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = LN})
#define _EXP(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = EXP})
#define _SIN(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = SIN})
#define _COS(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = COS})
#define _TAN(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = TAN})
#define _COT(right)         createNode(_NUM(-1), right, OPER, (NodeValue){.operation_type = COT})

#endif
