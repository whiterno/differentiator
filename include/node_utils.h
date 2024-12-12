#ifndef NODE_UTILS_H
#define NODE_UTILS_H

#include "bin_exp_tree.h"

Node* copyTree(Node* root);
bool compareTrees(Node* root1, Node* root2);
Node* makeParents(Node* root, Node* parent);

#endif
