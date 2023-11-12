#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter/interpreter.h"
#include "parser/parser.h"

int main() {
    char* regex_str = "a(b|c)d?e*f+g";
    char* str = "abdeffg";

    printf("input: %s\n", regex_str);

    Graph* g = parse(regex_str);

    FILE* fp1 = fopen("graph_unoptimized.dot", "w");
    graph_print_dot(g, fp1, NULL);
    fclose(fp1);

    graph_optimize(g);

    FILE* fp2 = fopen("graph_optimized.dot", "w");
    graph_print_dot(g, fp2, NULL);
    fclose(fp2);

    printf("%d\n", match(g, str));

    graph_destroy(g);

    return 0;
}