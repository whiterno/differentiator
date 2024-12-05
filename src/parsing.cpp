#include <stdio.h>
#include <assert.h>

#include "utils.h"

#include "bin_exp_tree.h"

Node* recursiveDescent(const char* filename){
    assert(filename);

    char* expression = NULL;
    readFile(filename, &expression);
}
