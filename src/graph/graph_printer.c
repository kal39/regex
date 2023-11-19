#include <stdlib.h>

#include "graph_printer.h"

GraphPrinter* graph_printer_open(FILE* fp, char* title) {
    GraphPrinter* gp = malloc(sizeof(GraphPrinter));
    gp->fp = fp;
    gp->i = 0;

    fprintf(fp, "digraph {\n");
    fprintf(fp, "  rankdir=LR;\n");
    fprintf(fp, "  labelloc=\"t\"\n");
    fprintf(fp, "  label=\"%s\"\n", title ? title : "");

    return gp;
}

void graph_printer_close(GraphPrinter* gp) {
    fprintf(gp->fp, "}\n");
    free(gp);
}

void graph_printer_print(GraphPrinter* gp, Graph* g, bool* state, char* label) {
    fprintf(gp->fp, "  subgraph cluster_%d {\n", gp->i);
    fprintf(gp->fp, "    label=\"%s\"\n", label ? label : "");
    fprintf(
        gp->fp,
        "    \"%d_s\" [label=\"\",shape=none,height=.0,width=.0]\n",
        gp->i
    );

    inter_nodes(g, i) {
        bool hasIn = false;
        inter_nodes(g, j) {
            if (graph_get(g, j)->out1 == i || graph_get(g, j)->out2 == i)
                hasIn = true;
        }

        if (!hasIn && i != g->start) continue;

        fprintf(gp->fp, "    \"%d_%d\" [label=\"", gp->i, i);
        character_print(NodeList_get(g->nodes, i).c, gp->fp);
        fprintf(
            gp->fp,
            "\", style=%s, shape=%s, fixedsize=true, width=0.7, height=0.7];\n",
            state && state[i] ? "filled" : "empty",
            i == g->end ? "doublecircle" : "circle"
        );

        if (i == g->start)
            fprintf(gp->fp, "    \"%d_s\" -> \"%d_%d\";\n", gp->i, gp->i, i);

        if (graph_get(g, i)->out1)
            fprintf(
                gp->fp,
                "    \"%d_%d\" -> \"%d_%d\";\n",
                gp->i,
                i,
                gp->i,
                graph_get(g, i)->out1
            );

        if (graph_get(g, i)->out2)
            fprintf(
                gp->fp,
                "    \"%d_%d\" -> \"%d_%d\";\n",
                gp->i,
                i,
                gp->i,
                graph_get(g, i)->out2
            );
    }

    fprintf(gp->fp, "  };\n");

    if (gp->i != 0) {
        fprintf(
            gp->fp,
            "  \"%d_1\" -> \"%d_s\" [constraint=false, style=invis];\n",
            gp->i - 1,
            gp->i
        );
    }
    gp->i++;
}