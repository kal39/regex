#include "graph.h"

#include <stdlib.h>

GEN_LIST_IMPL(NodeList, Node)

Graph* graph_create() {
    Graph* graph = malloc(sizeof(Graph));
    graph->nodes = NodeList_create(0, (Node){0, NULL, NULL});
    graph->start = NULL;
    return graph;
}

void graph_destroy(Graph* graph) {
    NodeList_destroy(graph->nodes);
    free(graph);
}

Node* graph_new_node(Graph* graph, char c, Node* out1, Node* out2) {
    Node node = (Node){c, out1, out2};
    NodeList_push(graph->nodes, node);
    return NodeList_peek_ref(graph->nodes);
}

Node* graph_get_start(Graph* graph) {
    return graph->start;
}

void graph_set_start(Graph* graph, Node* start) {
    graph->start = start;
}

bool graph_optimize(Graph* graph) {
    bool workDone = false;

    for (int i = 0; i < NodeList_len(graph->nodes); i++) {
        Node* node = NodeList_get_ref(graph->nodes, i);

        // skip current node if it's empty and has one out
        if (node->c == 0 && (node->out1 == NULL || node->out2 == NULL)) {
            Node* out = node->out1 ? node->out1 : node->out2;

            for (int j = 0; j < NodeList_len(graph->nodes); j++) {
                Node* _node = NodeList_get_ref(graph->nodes, j);
                if (_node->out1 == node) {
                    _node->out1 = out;
                    workDone = true;
                }
                if (_node->out2 == node) {
                    _node->out2 = out;
                    workDone = true;
                }
            }

            node->out1 = NULL;
            node->out2 = NULL;
        }

        // if current node only has one out, skip next node if it's empty
        if (node->out1 == NULL || node->out2 == NULL) {
            Node* out = node->out1 ? node->out1 : node->out2;

            if (out && out->c == 0) {
                node->out1 = out->out1;
                node->out2 = out->out2;
                workDone = true;
            }
        }

        // skip current node if it has no in, and isn't the start node
        if (node->out1 || node->out2) {
            bool hasIn = false;
            for (int j = 0; j < NodeList_len(graph->nodes); j++) {
                Node* _node = NodeList_get_ref(graph->nodes, j);
                if (_node->out1 == node || _node->out2 == node) hasIn = true;
            }

            if (!hasIn && node != graph->start) {
                node->out1 = NULL;
                node->out2 = NULL;
                workDone = true;
            }
        }
    }

    return workDone;
}

void graph_print(Graph* graph, FILE* stream) {
    fprintf(stream, "digraph {\nrankdir=LR;\n");

    for (int i = 0; i < NodeList_len(graph->nodes); i++) {
        Node* node = NodeList_get_ref(graph->nodes, i);

        bool hasIn = false;
        for (int j = 0; j < NodeList_len(graph->nodes); j++) {
            Node* _node = NodeList_get_ref(graph->nodes, j);
            if (_node->out1 == node || _node->out2 == node) hasIn = true;
        }

        if (!hasIn && node != graph->start) continue;

        fprintf(
            stream,
            "\"%p\"[label=\"%c\", shape=%s];\n",
            (void*)node,
            node->c ? node->c : ' ',
            node == graph->start ? "doublecircle" : "circle"
        );
        if (node->out1)
            fprintf(
                stream,
                "\"%p\" -> \"%p\";\n",
                (void*)node,
                (void*)node->out1
            );
        if (node->out2)
            fprintf(
                stream,
                "\"%p\" -> \"%p\";\n",
                (void*)node,
                (void*)node->out2
            );
    }

    fprintf(stream, "}");
}