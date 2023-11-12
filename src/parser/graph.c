#include "graph.h"

#include <stdlib.h>

static bool is_empty(Graph* g, NodeID id) {
    return g->nodes[id].c == 0;
}

static int get_out_count(Graph* g, NodeID id) {
    return (g->nodes[id].out1 ? 1 : 0) + (g->nodes[id].out2 ? 1 : 0);
}

Graph* graph_create() {
    Graph* g = malloc(sizeof(Graph));
    g->nodeCount = 1; // idx 0 used for NULL
    g->nodeCap = 2;
    g->nodes = malloc(sizeof(Node) * g->nodeCap);
    g->start = 0;
    g->end = 0;
    return g;
}

void graph_destroy(Graph* g) {
    free(g->nodes);
    free(g);
}

Node graph_get(Graph* g, NodeID id) {
    return g->nodes[id];
}

void graph_set_out1(Graph* g, NodeID id, NodeID out1) {
    g->nodes[id].out1 = out1;
}

void graph_set_out2(Graph* g, NodeID id, NodeID out2) {
    g->nodes[id].out2 = out2;
}

NodeID graph_add(Graph* g, char c, NodeID out1, NodeID out2) {
    Node node = (Node){c, out1, out2, false};

    if (g->nodeCount >= g->nodeCap) {
        g->nodeCap *= 2;
        g->nodes = realloc(g->nodes, sizeof(Node) * g->nodeCap);
    }

    g->nodes[g->nodeCount++] = node;
    return g->nodeCount - 1;
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

void graph_optimize(Graph* g) {
    while (true) {
        bool workDone = false;

        for (int i = 1; i < g->nodeCount; i++) {
            if (i == g->end) continue;

            // skip current node if it's empty and has 1 out
            if (is_empty(g, i) && get_out_count(g, i) == 1) {
                NodeID next = graph_get(g, i).out1 ? graph_get(g, i).out1
                                                   : graph_get(g, i).out2;

                for (int j = 1; j < g->nodeCount; j++) {
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
                for (int j = 1; j < g->nodeCount; j++) {
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

void graph_print_txt(Graph* g, FILE* fp) {
    fprintf(fp, "graph %p:\n", (void*)g);
    fprintf(fp, "- start: %d\n", g->start);
    fprintf(fp, "- end:   %d\n", g->end);
    fprintf(fp, "- nodes(%d):\n", g->nodeCount);

    for (int i = 1; i < g->nodeCount; i++) {
        fprintf(fp, "  - node %d:\n", i);
        fprintf(fp, "    - c:    '%c'\n", g->nodes[i].c);
        fprintf(fp, "    - out1: %d\n", g->nodes[i].out1);
        fprintf(fp, "    - out2: %d\n", g->nodes[i].out2);
    }
}

void graph_print_dot(Graph* g, FILE* fp) {
    fprintf(fp, "digraph {\n");
    fprintf(fp, "rankdir=LR;\n");
    fprintf(fp, "\"start\" [label=\"\", shape=none,height=.0,width=.0]\n");

    for (int i = 1; i < g->nodeCount; i++) {
        bool hasIn = false;
        for (int j = 1; j < g->nodeCount; j++) {
            if (graph_get(g, j).out1 == i || graph_get(g, j).out2 == i)
                hasIn = true;
        }

        if (!hasIn && i != g->start) continue;

        fprintf(
            fp,
            "%d [label=\"%c\", shape=%s];\n",
            i,
            g->nodes[i].c ? g->nodes[i].c : ' ',
            i == g->end ? "doublecircle" : "circle"
        );

        if (i == g->start) fprintf(fp, "\"start\" -> %d;\n", i);

        if (graph_get(g, i).out1)
            fprintf(fp, "%d -> %d;\n", i, graph_get(g, i).out1);

        if (graph_get(g, i).out2)
            fprintf(fp, "%d -> %d;\n", i, graph_get(g, i).out2);
    }

    fprintf(fp, "}");
}