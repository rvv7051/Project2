// @author: rvv7051

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"

/// The head of the linked list symbol table.
/// This is only visible inside this file.
static symbol_t *symtab_head = NULL;

/// Helper to trim leading whitespace and return first non-space char
static int first_nonspace_is_hash(const char *line) {
    const char *p = line;
    while (*p == ' ' || *p == '\t') {
        p++;
    }
    return (*p == '#');
}

void build_table(char *filename) {
    // Start with an empty table
    symtab_head = NULL;

    // No filename → empty table, no error
    if (filename == NULL) {
        return;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        // Error loading symbol table as per spec
        perror(filename);
        exit(EXIT_FAILURE);
    }

    char buf[BUFLEN];

    while (fgets(buf, BUFLEN, fp) != NULL) {
        // Skip comment lines (starting with # after optional whitespace)
        if (first_nonspace_is_hash(buf)) {
            continue;
        }

        // Try to parse either:
        //   type name value   (3 tokens)
        //   or
        //   name value        (2 tokens, as in sym.1 from the project spec)
        char type[BUFLEN];
        char name[BUFLEN];
        int val;

        int n = sscanf(buf, "%s %s %d", type, name, &val);
        if (n == 3) {
            // type name value → we only care about name + value
            create_symbol(name, val);
        } else {
            // maybe it's just "name value"
            n = sscanf(buf, "%s %d", name, &val);
            if (n == 2) {
                create_symbol(name, val);
            }
            // If it doesn't match either form, we ignore the line.
            // For this assignment, they say the file is well-formed,
            // so this case should not happen.
        }
    }

    fclose(fp);
}

void dump_table(void) {
    if (symtab_head == NULL) {
        // Spec says: if symbol table is empty, print nothing at all
        return;
    }

    printf("SYMBOL TABLE:\n");
    for (symbol_t *curr = symtab_head; curr != NULL; curr = curr->next) {
        printf("\tName: %s, Value: %d\n", curr->var_name, curr->val);
    }
}

symbol_t *lookup_table(char *variable) {
    for (symbol_t *curr = symtab_head; curr != NULL; curr = curr->next) {
        if (strcmp(curr->var_name, variable) == 0) {
            return curr;
        }
    }
    return NULL;
}

symbol_t *create_symbol(char *name, int val) {
    symbol_t *sym = malloc(sizeof(symbol_t));
    if (sym == NULL) {
        fprintf(stderr, "No room in symbol table\n");
        return NULL;
    }

    sym->var_name = malloc(strlen(name) + 1);
    if (sym->var_name == NULL) {
        fprintf(stderr, "No room in symbol table\n");
        free(sym);
        return NULL;
    }
    strcpy(sym->var_name, name);

    sym->val = val;
    sym->next = symtab_head;
    symtab_head = sym;

    return sym;
}

void free_table(void) {
    symbol_t *curr = symtab_head;
    while (curr != NULL) {
        symbol_t *next = curr->next;
        free(curr->var_name);
        free(curr);
        curr = next;
    }
    symtab_head = NULL;
}
