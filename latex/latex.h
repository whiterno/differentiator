#ifndef LATEX_H
#define LATEX_H

#include "bin_exp_tree.h"

const char TEX_FILENAME[] = "latex/texDump.tex";

enum TexMode{
    FULL_TREE,
    SUBTREE,
};

int beginTexDump(FILE* tex_dump);
int endTexDump(FILE* tex_dump);
int texDumpTree(FILE* tex_dump, Node* node, TexMode mode);

#endif
