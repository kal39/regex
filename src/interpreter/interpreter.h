#pragma once

#include <stdbool.h>

#include "graph/graph_printer.h"

bool match(Graph* regex, char* str, GraphPrinter* gp);