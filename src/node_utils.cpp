#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "node_utils.h"
#include "utils.h"

#include "bin_exp_tree.h"

static bool compareNodes(Node* node1, Node* node2);
static Node* copyNode(Node* node);

Node* copyTree(Node* root){
    if (root == NULL) return NULL;

    Node* new_root      = copyNode(root);
    new_root->left      = copyTree(root->left);
    new_root-> right    = copyTree(root->right);

    return new_root;
}

bool compareTrees(Node* root1, Node* root2){
    if (compareNodes(root1, root2)){
        if (root1 == NULL) return true;

        return compareTrees(root1->left, root2->left) && compareTrees(root1->right, root2->right);
    }

    return false;
}

Node* makeParents(Node* root, Node* parent){
    if (root == NULL) return root;

    root->parent = parent;

    makeParents(root->left, root);
    makeParents(root->right, root);

    return root;
}

static bool compareNodes(Node* node1, Node* node2){
    if ((node1 == NULL && node2 != NULL) ||
        (node1 != NULL && node2 == NULL))   return false;
    if  (node1 == NULL && node2 == NULL)    return true;

    if (node1->type == node2->type){
        if (node1->type == OPER){
            return node1->value.operation_type == node2->value.operation_type;
        }
        if (node1->type == NUM){
            return cmpf(node1->value.number, node2->value.number);
        }
        if (node1->type == VAR){
            return node1->value.variable == node2->value.variable;
        }
    }

    return false;
}

static Node* copyNode(Node* node){
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    memcpy(new_node, node, sizeof(Node));

    return new_node;
}
