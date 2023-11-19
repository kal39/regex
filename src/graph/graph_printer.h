#pragma once

#include <stdio.h>

#include "graph.h"

typedef struct GraphPrinter {
    FILE* fp;
    int i;
} GraphPrinter;

GraphPrinter* graph_printer_open(FILE* fp, char* title);
void graph_printer_close(GraphPrinter* gp);

void graph_printer_print(GraphPrinter* gp, Graph* g, bool* state, char* label);