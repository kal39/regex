#pragma once

#include <stdbool.h>

#include "list/list.h"

typedef int NodeID;

typedef struct Node {
    char c;
    NodeID out1;
    NodeID out2;
} Node;

GEN_LIST_DEF(NodeList, Node)

typedef struct Graph {
    NodeList* nodes;
    NodeID start;
    NodeID end;
} Graph;

#define inter_nodes(g, id) for (NodeID id = 1; id < graph_size(g); id++)

Graph* graph_create();
void graph_destroy(Graph* g);

NodeID graph_add(Graph* g, char c, NodeID out1, NodeID out2);
Node* graph_get(Graph* g, NodeID id);

int graph_size(Graph* g);

void graph_optimize(Graph* g);
