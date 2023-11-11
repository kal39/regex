#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler/compiler.h"
#include "parser/parser.h"

int main() {
    char* regex_str = "ab?c*d+e";

    printf("input: %s\n", regex_str);

    Scanner* s = scanner_create(regex_str);

    printf("scan:  ");
    for (Token t = scanner_next(s); t.type != TOK_END; t = scanner_next(s))
        printf(t.type == TOK_CHAR ? "%c" : "%c", t.c);
    printf("\n");

    scanner_destroy(s);

    Parser* p = parser_create(regex_str);

    printf("parse: ");
    for (Token t = parser_next(p); t.type != TOK_END; t = parser_next(p))
        printf(t.type == TOK_CHAR ? "%c" : "%c", t.c);
    printf("\n");

    parser_destroy(p);

    Graph* g = compile(regex_str);

    FILE* fp1 = fopen("graph_unoptimized.dot", "w");
    graph_print(g, fp1);
    fclose(fp1);

    while (graph_optimize(g)) {
    }

    FILE* fp2 = fopen("graph_optimized.dot", "w");
    graph_print(g, fp2);
    fclose(fp2);

    graph_destroy(g);

    return 0;
}