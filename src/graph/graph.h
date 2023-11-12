#pragma once

#include <stdbool.h>

#include "node.h"

typedef struct Graph {
    NodeList* nodes;
    NodeID start;
    NodeID end;
} Graph;

Graph* graph_create();
void graph_destroy(Graph* g);

NodeID graph_add(Graph* g, char c, NodeID out1, NodeID out2);

Node graph_get(Graph* g, NodeID id);

int graph_size(Graph* g);

void graph_set_out1(Graph* g, NodeID id, NodeID out1);
void graph_set_out2(Graph* g, NodeID id, NodeID out2);

NodeID graph_get_start(Graph* g);
void graph_set_start(Graph* g, NodeID start);

NodeID graph_get_end(Graph* g);
void graph_set_end(Graph* g, NodeID end);

NodeID graph_iter_begin(Graph* g);
void graph_iter(Graph* g, NodeID* idx);

void graph_optimize(Graph* g);
