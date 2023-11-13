#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph/graph_printer.h"
#include "interpreter/interpreter.h"
#include "parser/parser.h"

int main() {
    char* regex_str = "a1-5^1-5(b|c)d?e*f+g";
    char* str = "a1zbdeffg";

    printf("input: %s\n", regex_str);

    Graph* g = parse(regex_str);

    FILE* fp1 = fopen("match_unoptimized.dot", "w");
    GraphPrinter* gp1 = graph_printer_open(fp1);
    printf("%d\n", match(g, str, gp1));
    graph_printer_close(gp1);
    fclose(fp1);

    graph_optimize(g);

    FILE* fp2 = fopen("match_optimized.dot", "w");
    GraphPrinter* gp2 = graph_printer_open(fp2);
    printf("%d\n", match(g, str, gp2));
    graph_printer_close(gp2);
    fclose(fp2);

    graph_destroy(g);

    return 0;
}