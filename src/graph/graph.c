#include <stdlib.h>

#include "graph.h"

static bool is_empty(Graph* g, NodeID id) {
    return NodeList_get(g->nodes, id).c == 0;
}

static int get_out_count(Graph* g, NodeID id) {
    return (NodeList_get(g->nodes, id).out1 ? 1 : 0)
         + (NodeList_get(g->nodes, id).out2 ? 1 : 0);
}

Graph* graph_create() {
    Graph* g = malloc(sizeof(Graph));
    g->nodes = NodeList_create(0, (Node){0, 0, 0});
    NodeList_push(g->nodes, (Node){0, 0, 0}); // idx 0 is empty
    g->start = 0;
    g->end = 0;
    return g;
}

void graph_destroy(Graph* g) {
    free(g->nodes);
    free(g);
}

Node graph_get(Graph* g, NodeID id) {
    return NodeList_get(g->nodes, id);
}

int graph_size(Graph* g) {
    return NodeList_len(g->nodes);
}

void graph_set_out1(Graph* g, NodeID id, NodeID out1) {
    Node node = NodeList_get(g->nodes, id);
    node.out1 = out1;
    NodeList_set(g->nodes, id, node);
}

void graph_set_out2(Graph* g, NodeID id, NodeID out2) {
    Node node = NodeList_get(g->nodes, id);
    node.out2 = out2;
    NodeList_set(g->nodes, id, node);
}

NodeID graph_add(Graph* g, char c, NodeID out1, NodeID out2) {
    NodeList_push(g->nodes, (Node){c, out1, out2});
    return NodeList_len(g->nodes) - 1;
}

NodeID graph_get_start(Graph* g) {
    return g->start;
}

void graph_set_start(Graph* g, NodeID start) {
    g->start = start;
}

NodeID graph_get_end(Graph* g) {
    return g->end;
}

void graph_set_end(Graph* g, NodeID end) {
    g->end = end;
}

NodeID graph_iter_begin(Graph* g) {
    return 1;
}

void graph_iter(Graph* g, NodeID* idx) {
    *idx = *idx >= NodeList_len(g->nodes) - 1 ? 0 : *idx + 1;
}

void graph_optimize(Graph* g) {
    while (true) {
        bool workDone = false;

        for (NodeID i = graph_iter_begin(g); i; graph_iter(g, &i)) {
            if (i == g->end) continue;

            // skip current node if it's empty and has 1 out
            if (is_empty(g, i) && get_out_count(g, i) == 1) {
                NodeID next = graph_get(g, i).out1 ? graph_get(g, i).out1
                                                   : graph_get(g, i).out2;

                for (NodeID j = graph_iter_begin(g); j; graph_iter(g, &j)) {
                    if (graph_get(g, j).out1 == i) {
                        graph_set_out1(g, j, next);
                        workDone = true;
                    }
                    if (graph_get(g, j).out2 == i) {
                        graph_set_out2(g, j, next);
                        workDone = true;
                    }
                }

                graph_set_out2(g, i, 0);
                graph_set_out2(g, i, 0);
            }

            // if current node has 1 out, skip next node if it's empty
            if (get_out_count(g, i) == 1) {
                NodeID next = graph_get(g, i).out1 ? graph_get(g, i).out1
                                                   : graph_get(g, i).out2;

                if (next && next != g->end && is_empty(g, next)) {
                    graph_set_out1(g, i, graph_get(g, next).out1);
                    graph_set_out2(g, i, graph_get(g, next).out2);
                    workDone = true;
                }
            }

            // skip current node if it has 0 in
            if (i != g->start && get_out_count(g, i) != 0) {
                bool hasIn = false;
                for (NodeID j = graph_iter_begin(g); j; graph_iter(g, &j)) {
                    if (graph_get(g, j).out1 == i || graph_get(g, j).out2 == i)
                        hasIn = true;
                }

                if (!hasIn) {
                    graph_set_out1(g, i, 0);
                    graph_set_out2(g, i, 0);
                    workDone = true;
                }
            }
        }

        if (!workDone) break;
    }
}
