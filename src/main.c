#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph/graph_printer.h"
#include "interpreter/interpreter.h"
#include "parser/parser.h"

int main() {
    char* regex_str = "a(b|c)d?e*f+g";
    char* str = "abdeffg";

    printf("input: %s\n", regex_str);

    Graph* g = parse(regex_str);

    FILE* fp1 = fopen("graph.dot", "w");
    GraphPrinter* gp1 = graph_printer_open(fp1);

    graph_printer_print(gp1, g, NULL, "unoptimized");

    graph_optimize(g);

    graph_printer_print(gp1, g, NULL, "optimized");

    graph_printer_close(gp1);

    FILE* fp2 = fopen("match_log.dot", "w");
    GraphPrinter* gp2 = graph_printer_open(fp2);

    printf("%d\n", match(g, str, gp2));

    graph_printer_close(gp2);

    graph_destroy(g);

    return 0;
}