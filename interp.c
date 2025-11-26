#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interp.h"
#include "symtab.h"   /* build_table, dump_table, free_table prototypes */

#define LINEBUF 1024

int main(int argc, char *argv[]) {
    const char *symfile = NULL;

    /* Optional symbol file argument */
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [symfile]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 2) {
        symfile = argv[1];
    }

    /* Build the symbol table (from file if given, or empty otherwise) */
    build_table(symfile);

    /* Print initial symbol table if not empty */
    dump_table();

    /* Start the read–eval–print loop (eval/parse comes later) */
    printf("Enter postfix expressions (CTRL-D to exit):\n");

    char line[LINEBUF];

    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            /* EOF / CTRL-D */
            printf("\n");
            break;
        }

        /* Skip leading whitespace */
        char *p = line;
        while (*p == ' ' || *p == '\t') {
            p++;
        }

        /* If the (trimmed) line starts with '#', treat as full-line comment */
        if (*p == '#') {
            continue;
        }

        /* If there is a '#' later in the line, treat it as start of inline comment */
        char *hash = strchr(p, '#');
        if (hash != NULL) {
            *hash = '\0';
        }

        /* Trim trailing newline from the part we actually care about */
        size_t len = strlen(p);
        if (len > 0 && p[len - 1] == '\n') {
            p[len - 1] = '\0';
        }

        /* TEMP: just echo the cleaned line (no parsing/eval yet) */
        if (*p != '\0') {
            printf("%s\n", p);
        }
    }

    /* Print final symbol table (after assignments once you implement eval) */
    dump_table();
    free_table();

    return EXIT_SUCCESS;
}

