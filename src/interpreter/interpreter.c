#include <stdlib.h>

#include "interpreter.h"

bool match(Graph* g, char* str, GraphPrinter* gp) {
    // init states
    bool* old_state = malloc(sizeof(bool) * graph_size(g));
    bool* new_state = malloc(sizeof(bool) * graph_size(g));
    inter_nodes(g, i) new_state[i] = false;
    new_state[g->start] = true;

    for (int i = 0, k = 0; str[i]; k++) {
        // swap state buffers
        bool* tmp = old_state;
        old_state = new_state;
        new_state = tmp;

        if (gp) {
            char label[256];
            sprintf(label, "iteration %d.%d '%c'", i, k, str[i]);
            graph_printer_print(gp, g, old_state, label);
        }

        // early termination
        bool hasActive = false;
        inter_nodes(g, j) {
            if (old_state[j]) hasActive = true;
        }
        if (!hasActive) break;

        // clear new state
        inter_nodes(g, j) {
            new_state[j] = false;
            if (k != 0 && !character_is_empty(graph_get(g, j)->c))
                new_state[j] = old_state[j];
        }

        // iterate
        bool emptyNode = false;
        inter_nodes(g, j) {
            if (!old_state[j]) continue;

            if ((k == 0 && character_in(graph_get(g, j)->c, str[i]))
                || character_is_empty(graph_get(g, j)->c)) {
                NodeID out1 = graph_get(g, j)->out1;
                NodeID out2 = graph_get(g, j)->out2;

                new_state[out1] = true;
                new_state[out2] = true;

                if (out1 && character_is_empty(graph_get(g, out1)->c))
                    emptyNode = true;
                if (out2 && character_is_empty(graph_get(g, out2)->c))
                    emptyNode = true;
            }
        }

        if (!emptyNode) {
            i++;
            k = -1;
        }
    }

    bool res = old_state[g->end];

    free(old_state);
    free(new_state);

    return res;
}