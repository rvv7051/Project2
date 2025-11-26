#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree_node.h"

static char *dup_token(const char *token) {
    char *copy = malloc(strlen(token) + 1);
    if (copy == NULL) {
        fprintf(stderr, "Error: out of memory duplicating token\n");
        return NULL;
    }
    strcpy(copy, token);
    return copy;
}

tree_node_t *make_leaf(exp_type_t exp_type, char *token) {
    if (token == NULL) {
        fprintf(stderr, "Error: NULL token passed to make_leaf\n");
        return NULL;
    }

    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (node == NULL) {
        fprintf(stderr, "Error: out of memory allocating tree_node (leaf)\n");
        return NULL;
    }

    char *tok_copy = dup_token(token);
    if (tok_copy == NULL) {
        free(node);
        return NULL;
    }

    leaf_node_t *leaf = malloc(sizeof(leaf_node_t));
    if (leaf == NULL) {
        fprintf(stderr, "Error: out of memory allocating leaf_node\n");
        free(tok_copy);
        free(node);
        return NULL;
    }

    leaf->exp_type = exp_type;

    node->type = LEAF;
    node->token = tok_copy;
    node->node  = leaf;

    return node;
}

tree_node_t *make_interior(op_type_t op, char *token,
                           tree_node_t *left, tree_node_t *right) {
    if (token == NULL) {
        fprintf(stderr, "Error: NULL token passed to make_interior\n");
        return NULL;
    }

    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (node == NULL) {
        fprintf(stderr, "Error: out of memory allocating tree_node (interior)\n");
        return NULL;
    }

    char *tok_copy = dup_token(token);
    if (tok_copy == NULL) {
        free(node);
        return NULL;
    }

    interior_node_t *in = malloc(sizeof(interior_node_t));
    if (in == NULL) {
        fprintf(stderr, "Error: out of memory allocating interior_node\n");
        free(tok_copy);
        free(node);
        return NULL;
    }

    in->op    = op;
    in->left  = left;
    in->right = right;

    node->type  = INTERIOR;
    node->token = tok_copy;
    node->node  = in;

    return node;
}
















