#ifndef READFILE_H
#define READFILE_H

#include "../lib/bin_exp_tree/include/bin_exp_tree.h"

enum ReadMode{
    PREFIX,
    INFIX,
    POSTFIX,
};

int readExpression(BinExpTree* tree, const char* filename, ReadMode mode);

#endif
