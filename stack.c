// @author: rvv7051
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "stack_node.h"

stack_t *make_stack(void) {
    stack_t *stack = malloc(sizeof(stack_t));
    if (stack == NULL) {
        fprintf(stderr, "Failed to allocate stack\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    return stack;
}

void push(stack_t *stack, void *data) {
    if (stack == NULL) {
        fprintf(stderr, "push called with NULL stack\n");
        exit(EXIT_FAILURE);
    }

    stack_node_t *node = malloc(sizeof(stack_node_t));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate stack node\n");
        exit(EXIT_FAILURE);
    }

    node->data = data;
    node->next = stack->top;
    stack->top = node;
}

void *top(stack_t *stack) {
    if (stack == NULL || stack->top == NULL) {
        fprintf(stderr, "attempt to retrieve the top of an empty stack\n");
        exit(EXIT_FAILURE);
    }
    return stack->top->data;
}

void pop(stack_t *stack) {
    if (stack == NULL || stack->top == NULL) {
        fprintf(stderr, "attempt to pop from an empty stack\n");
        exit(EXIT_FAILURE);
    }

    stack_node_t *old_top = stack->top;
    stack->top = old_top->next;

    /* NOTE: we do NOT free old_top->data here, because the caller
       (e.g., tokenizer/parser) owns the data (like token strings). */
    free(old_top);
}

int empty_stack(stack_t *stack) {
    if (stack == NULL) {
        /* A NULL stack is effectively empty */
        return 1;
    }
    return (stack->top == NULL);
}

void free_stack(stack_t *stack) {
    if (stack == NULL) {
        return;
    }

    stack_node_t *curr = stack->top;
    while (curr != NULL) {
        stack_node_t *next = curr->next;
        /* Again, do NOT free curr->data here; the caller owns it. */
        free(curr);
        curr = next;
    }

    free(stack);
}
