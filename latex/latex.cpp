#include <stdio.h>

#include "latex.h"
#include "utils.h"

#include "bin_exp_tree.h"

static int texDumpSubtree(FILE* tex_dump, Node* node, int oper_priority);
static int fprintNode(FILE* tex_dump, Node* node);
static int fprintOper(FILE* tex_dump, Operations oper);
static int getPriority(Node* node);
static bool isFunc(Operations operation);


int beginTexDump(FILE* tex_dump){
    if (tex_dump == NULL){
        printf("No such file\n");
        return FILE_NOT_OPEN;
    }

    fprintf(tex_dump, "\\documentclass{article}\n");
    fprintf(tex_dump, "\\usepackage{ucs}\n");
    fprintf(tex_dump, "\\usepackage[utf8x]{inputenc}\n");
    fprintf(tex_dump, "\\usepackage[russian]{babel}\n");
    fprintf(tex_dump, "\\usepackage{amsmath}\n");

    fprintf(tex_dump, "\\begin{document}\n\n");

    return NO_ERROR;
}

int endTexDump(FILE* tex_dump){
    if (tex_dump == NULL){
        printf("No such file\n");
        return FILE_NOT_OPEN;
    }

    fprintf(tex_dump, "\\end{document}\n");

    fclose(tex_dump);

    return NO_ERROR;
}

int texDumpTree(FILE* tex_dump, Node* node, TexMode mode){
    if (node == NULL) return NO_ERROR;

    if (mode == FULL_TREE){
        while (node->parent != NULL) node = node->parent;
    }

    fprintf(tex_dump, "$$");
    texDumpSubtree(tex_dump, node, getPriority(node));
    fprintf(tex_dump, "$$\n");

    return NO_ERROR;
}

static int texDumpSubtree(FILE* tex_dump, Node* node, int oper_priority){
    if (node->left == NULL){
        fprintNode(tex_dump, node);

        return NO_ERROR;
    }

    if (node->type == OPER && isFunc(node->value.operation_type)){
        Operations oper = node->value.operation_type;

        if (oper == LOG){
            fprintf(tex_dump, "log_{");
            texDumpSubtree(tex_dump, node->left, getPriority(node->left));
            fprintf(tex_dump, "}(");
            texDumpSubtree(tex_dump, node->right, getPriority(node->right));
            fprintf(tex_dump, ")");
        }
        else{
            fprintOper(tex_dump, oper);
            texDumpSubtree(tex_dump, node->right, getPriority(node->right));
            fprintf(tex_dump, ")}");
        }

        return NO_ERROR;
    }

    if (oper_priority > getPriority(node->left)){
        fprintf(tex_dump, "{(");
        texDumpSubtree(tex_dump, node->left, getPriority(node->left));
        fprintf(tex_dump, ")}");
    }
    else{
        texDumpSubtree(tex_dump, node->left, getPriority(node->left));
    }

    fprintNode(tex_dump, node);

    if (oper_priority > getPriority(node->right) || oper_priority == 3){
        if (oper_priority == 3){
            fprintf(tex_dump, "{");
            texDumpSubtree(tex_dump, node->right, getPriority(node->right));
            fprintf(tex_dump, "}");
        }
        else{
            fprintf(tex_dump, "{(");
            texDumpSubtree(tex_dump, node->right, getPriority(node->right));
            fprintf(tex_dump, ")}");
        }
    }
    else{
        texDumpSubtree(tex_dump, node->right, getPriority(node->right));
    }

    return NO_ERROR;
}

static int fprintNode(FILE* tex_dump, Node* node){
    Operations par_oper = (node->parent == NULL) ? UNKNOWN_OPERATION : node->parent->value.operation_type;

    switch (node->type){
        case (NUM):
            if (cmpf(node->value.number, -1) == 1 && (par_oper == MULT || par_oper == DIV)){
                fprintf(tex_dump, "(%lg)", node->value.number);
            }
            else{
                fprintf(tex_dump, "%lg", node->value.number);
            }
            return NO_ERROR;
        case (VAR): fprintf(tex_dump, "%c", node->value.variable); return NO_ERROR;
        case (OPER): fprintOper(tex_dump, node->value.operation_type); return NO_ERROR;
    }
}

static int fprintOper(FILE* tex_dump, Operations oper){
    switch (oper){
        case (ADD): fprintf(tex_dump, "+"); break;
        case (SUB): fprintf(tex_dump, "-"); break;
        case (DIV): fprintf(tex_dump, "/"); break;
        case (MULT): fprintf(tex_dump, "*"); break;
        case (LN): fprintf(tex_dump, "ln_{("); break;
        case (EXP): fprintf(tex_dump, "e^{("); break;
        case (LOG): fprintf(tex_dump, "log_{("); break;
        case (POW): fprintf(tex_dump, "^"); break;
        case (SIN): fprintf(tex_dump, "sin{("); break;
        case (COS): fprintf(tex_dump, "cos{("); break;
        case (TAN): fprintf(tex_dump, "tan{("); break;
        case (COT): fprintf(tex_dump, "cot{("); break;
        default: break;
    }

    return NO_ERROR;
}

static int getPriority(Node* node){
    if (node->type == NUM || node->type == VAR){
        return 4;
    }

    switch(node->value.operation_type){
        case(ADD):  return 1;
        case(SUB):  return 1;
        case(MULT): return 2;
        case(DIV):  return 2;
        case(POW):  return 3;
        default:    return 5;
    }
}

static bool isFunc(Operations operation){
    switch (operation)
    {
    case(LOG): return true;
    case(LN):  return true;
    case(EXP): return true;
    case(SIN): return true;
    case(COS): return true;
    case(TAN): return true;
    case(COT): return true;
    default:
        return false;
    }
}
