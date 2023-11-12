#include <stdlib.h>

#include "graph_printer.h"

GraphPrinter* graph_printer_open(FILE* fp) {
    GraphPrinter* gp = malloc(sizeof(GraphPrinter));
    gp->fp = fp;
    gp->i = 0;

    fprintf(fp, "digraph {\n");
    fprintf(fp, "rankdir=LR;\n");

    return gp;
}

void graph_printer_close(GraphPrinter* gp) {
    fprintf(gp->fp, "}\n");
    free(gp);
}

void graph_printer_print(GraphPrinter* gp, Graph* g, bool* state, char* label) {
    fprintf(gp->fp, "subgraph cluster_%d {\n", gp->i);
    fprintf(gp->fp, "label=\"%s\"\n", label ? label : "");
    fprintf(
        gp->fp,
        "\"%d_s\" [label=\"\",shape=none,height=.0,width=.0]\n",
        gp->i
    );

    for (NodeID i = graph_iter_begin(g); i; graph_iter(g, &i)) {
        bool hasIn = false;
        for (NodeID j = graph_iter_begin(g); j; graph_iter(g, &j)) {
            if (graph_get(g, j).out1 == i || graph_get(g, j).out2 == i)
                hasIn = true;
        }

        if (!hasIn && i != g->start) continue;

        fprintf(
            gp->fp,
            "\"%d_%d\" [label=\"%c\", style=%s, shape=%s];\n",
            gp->i,
            i,
            NodeList_get(g->nodes, i).c ? NodeList_get(g->nodes, i).c : ' ',
            state && state[i] ? "filled" : "empty",
            i == g->end ? "doublecircle" : "circle"
        );

        if (i == g->start)
            fprintf(gp->fp, "\"%d_s\" -> \"%d_%d\";\n", gp->i, gp->i, i);

        if (graph_get(g, i).out1)
            fprintf(
                gp->fp,
                "\"%d_%d\" -> \"%d_%d\";\n",
                gp->i,
                i,
                gp->i,
                graph_get(g, i).out1
            );

        if (graph_get(g, i).out2)
            fprintf(
                gp->fp,
                "\"%d_%d\" -> \"%d_%d\";\n",
                gp->i,
                i,
                gp->i,
                graph_get(g, i).out2
            );
    }

    fprintf(gp->fp, "};\n");

    if (gp->i != 0) {
        fprintf(
            gp->fp,
            "\"%d_1\" -> \"%d_s\" [constraint=false, style=invis];\n",
            gp->i - 1,
            gp->i
        );
    }
    gp->i++;
}