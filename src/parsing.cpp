#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "lexer.h"
#include "parsing.h"

#include "bin_exp_tree.h"

Node* recursiveDescent(const char* filename){
    assert(filename);

    char* expression = NULL;
    readFile(filename, &expression);

    Token* tokens = lexer(expression);
    free(expression);

    return NULL;
}

