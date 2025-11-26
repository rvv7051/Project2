#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "stack.h"
#include "tree_node.h"

static int is_operator(const char *tok) {
    return strcmp(tok, "+") == 0 ||
           strcmp(tok, "-") == 0 ||
           strcmp(tok, "*") == 0 ||
           strcmp(tok, "/") == 0;
}

tree_node_t *parse(FILE *fp) {
    stack_t *stack = make_stack();
    char tok[256];

    while (fscanf(fp, "%255s", tok) == 1) {
        if (is_operator(tok)) {
            tree_node_t *right = (tree_node_t *)top(stack); pop(stack);
            tree_node_t *left = (tree_node_t *)top(stack); pop(stack);
            tree_node_t *node = make_internal_node(tok[0], left, right);
            push(stack, node);
        } else {
            int value = atoi(tok);
            tree_node_t *node = make_leaf_node(value);
            push(stack, node);
        }
    }

    if (empty_stack(stack)) {
        fprintf(stderr, "Parse error: empty stack at end\n");
        exit(EXIT_FAILURE);
    }

    tree_node_t *root = (tree_node_t *)top(stack);
    pop(stack);

    if (!empty_stack(stack)) {
        fprintf(stderr, "Parse error: stack not empty after parse\n");
        exit(EXIT_FAILURE);
    }

    free_stack(stack);
    return root;
}
