#include <stdio.h>

#include "read_file.h"
#include "simplification.h"
#include "node_utils.h"
#include "differentiator.h"
#include "parsing.h"
#include "latex.h"

#include "bin_exp_tree.h"
#include "dump_bet.h"

int main(){
    BinExpTree* tree = NULL;
    binExpTreeCtor(&tree INIT_BET(tree));
    BinExpTree* diff_tree = NULL;
    binExpTreeCtor(&diff_tree INIT_BET(diff_tree));

    tree->root = recursiveDescent("pars_expr");
    binExpTreeDump(DUMP_BET(tree), 0);
    simplifyTree(tree->root);
    binExpTreeDump(DUMP_BET(tree), 0);

    diff_tree->root = diff(tree->root);
    simplifyTree(diff_tree->root);
    binExpTreeDump(DUMP_BET(diff_tree), 0);

    FILE* tex_dump = fopen(TEX_FILENAME, "w");

    beginTexDump(tex_dump);
    texDumpTree(tex_dump, tree->root, FULL_TREE);
    texDumpTree(tex_dump, diff_tree->root, FULL_TREE);
    endTexDump(tex_dump);
}
