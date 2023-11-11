#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "list/list.h"

typedef struct Node {
    char c;
    struct Node* out1;
    struct Node* out2;
} Node;

GEN_LIST_DEF(NodeList, Node)

typedef struct Graph {
    NodeList* nodes;
    Node* start;
} Graph;

Graph* graph_create();

void graph_destroy(Graph* graph);

Node* graph_new_node(Graph* graph, char c, Node* out1, Node* out2);

Node* graph_get_start(Graph* graph);

void graph_set_start(Graph* graph, Node* start);

bool graph_optimize(Graph* graph);

void graph_print(Graph* graph, FILE* stream);