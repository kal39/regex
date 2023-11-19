#include <stdlib.h>

#include "graph.h"

static bool is_empty(Graph* g, NodeID id) {
    return character_is_empty(NodeList_get(g->nodes, id).c);
}

static int get_out_count(Graph* g, NodeID id) {
    return (NodeList_get(g->nodes, id).out1 ? 1 : 0)
         + (NodeList_get(g->nodes, id).out2 ? 1 : 0);
}

GEN_LIST_IMPL(NodeList, Node)

Graph* graph_create() {
    Graph* g = malloc(sizeof(Graph));
    g->nodes = NodeList_create(0, (Node){character_empty(), 0, 0});
    NodeList_push(g->nodes, (Node){character_empty(), 0, 0}); // idx 0 is empty
    g->start = 0;
    g->end = 0;
    return g;
}

void graph_destroy(Graph* g) {
    free(g->nodes);
    free(g);
}

NodeID graph_add(Graph* g, Character c, NodeID out1, NodeID out2) {
    NodeList_push(g->nodes, (Node){c, out1, out2});
    return NodeList_len(g->nodes) - 1;
}

Node* graph_get(Graph* g, NodeID id) {
    return NodeList_get_ref(g->nodes, id);
}

int graph_size(Graph* g) {
    return NodeList_len(g->nodes);
}

void graph_optimize(Graph* g) {
    // clean up edges
    while (true) {
        bool workDone = false;

        inter_nodes(g, i) {
            if (i == g->end) continue;

            // skip current node if it's empty and has 1 out
            if (is_empty(g, i) && get_out_count(g, i) == 1) {
                NodeID next = graph_get(g, i)->out1 ? graph_get(g, i)->out1
                                                    : graph_get(g, i)->out2;

                inter_nodes(g, j) {
                    if (graph_get(g, j)->out1 == i) {
                        graph_get(g, j)->out1 = next;
                        workDone = true;
                    }
                    if (graph_get(g, j)->out2 == i) {
                        graph_get(g, j)->out2 = next;
                        workDone = true;
                    }
                }

                graph_get(g, i)->out1 = 0;
                graph_get(g, i)->out2 = 0;
            }

            // if current node has 1 out, skip next node if it's empty
            if (get_out_count(g, i) == 1) {
                NodeID next = graph_get(g, i)->out1 ? graph_get(g, i)->out1
                                                    : graph_get(g, i)->out2;

                if (next && next != g->end && is_empty(g, next)) {
                    graph_get(g, i)->out1 = graph_get(g, next)->out1;
                    graph_get(g, i)->out2 = graph_get(g, next)->out2;
                    workDone = true;
                }
            }

            // skip current node if it has 0 in
            if (i != g->start && get_out_count(g, i) != 0) {
                bool hasIn = false;
                inter_nodes(g, j) {
                    if (graph_get(g, j)->out1 == i
                        || graph_get(g, j)->out2 == i)
                        hasIn = true;
                }

                if (!hasIn) {
                    graph_get(g, i)->out1 = 0;
                    graph_get(g, i)->out2 = 0;
                    workDone = true;
                }
            }
        }

        if (!workDone) break;
    }

    // remove empty nodes
    for (NodeID i = graph_size(g) - 1; i > 0; i--) {
        if (i != g->end && get_out_count(g, i) == 0) {
            NodeList_remove(g->nodes, i);

            inter_nodes(g, j) {
                if (graph_get(g, j)->out1 > i) graph_get(g, j)->out1--;
                if (graph_get(g, j)->out2 > i) graph_get(g, j)->out2--;
            }

            if (g->start > i) g->start--;
            if (g->end > i) g->end--;
        }
    }
}
