#include <stdio.h>

#include "read_file.h"
#include "simplification.h"

#include "bin_exp_tree.h"
#include "dump_bet.h"

int main(){
    BinExpTree* tree = NULL;
    binExpTreeCtor(&tree INIT_BET(tree));

    readExpression(tree, "expression.txt", INFIX);

    binExpTreeDump(DUMP_BET(tree), 0);

    Node res = getTreeValue(tree->root);
    printf("RES TYPE: %d\n", res.type);
    if (res.type == NUM){
        printf("RESULT: %lf\n", res.value.number);
    }
    else{
        printf("HAVE VARIABLES\n");
    }

    constCollapse(tree->root);
    binExpTreeDump(DUMP_BET(tree), 0);

    printf("HERE!");
    simpleCasesCollapse(tree->root);
    binExpTreeDump(DUMP_BET(tree), 0);

    // simpleCasesCollapse(tree->root);
    // binExpTreeDump(DUMP_BET(tree), 0);

}
