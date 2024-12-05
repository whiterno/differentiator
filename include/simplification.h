#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "bin_exp_tree.h"

void simplifyTree(Node* root);
Node getTreeValue(Node* root);
void constCollapse(Node* root);
void simpleCasesCollapse(Node* root);

#endif
