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

void graph_optimize(Graph* graph) {
    for (unsigned int i = 0; i < NodeList_len(graph->nodes); i++) {
        Node* node = NodeList_get_ref(graph->nodes, i);
        if (node->c == 0 && (node->out1 == NULL || node->out2 == NULL)) {
            Node* out = node->out1 ? node->out1 : node->out2;

            // skip node
            for (unsigned int j = 0; j < NodeList_len(graph->nodes); j++) {
                Node* _node = NodeList_get_ref(graph->nodes, j);
                if (_node->out1 == node) _node->out1 = out;
                if (_node->out2 == node) _node->out2 = out;
            }

            // unlink remove node
            node->out1 = NULL;
            node->out2 = NULL;
        }
    }
}

void graph_print(Graph* graph, FILE* stream) {
    fprintf(stream, "digraph {\nrankdir=LR;\n");

    for (unsigned int i = 0; i < NodeList_len(graph->nodes); i++) {
        Node* node = NodeList_get_ref(graph->nodes, i);
        fprintf(
            stream,
            "\"%p\"[label=\"%c\"];\n",
            (void*)node,
            node->c ? node->c : ' '
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