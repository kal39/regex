#include <stdlib.h>

#include "interpreter.h"

bool match(Graph* g, char* str, GraphPrinter* gp) {
    // init states
    bool* old_state = malloc(sizeof(bool) * graph_size(g));
    bool* new_state = malloc(sizeof(bool) * graph_size(g));
    for (NodeID i = 1; i < graph_size(g); i++) new_state[i] = false;
    new_state[graph_get_start(g)] = true;

    for (int i = 0; str[i]; i++) {
        // swap state buffers
        bool* tmp = old_state;
        old_state = new_state;
        new_state = tmp;

        // clear new state
        for (NodeID j = 1; j < graph_size(g); j++) new_state[j] = false;

        // iterate
        for (NodeID j = 1; j < graph_size(g); j++) {
            if (!old_state[j]) continue;

            if (graph_get(g, j).c == str[i] || graph_get(g, j).c == 0) {
                new_state[graph_get(g, j).out1] = true;
                new_state[graph_get(g, j).out2] = true;
            }
        }

        char label[256];
        sprintf(label, "iteration %d", i);
        if (gp) graph_printer_print(gp, g, old_state, label);
    }

    bool res = old_state[graph_get_end(g)];

    free(old_state);
    free(new_state);

    return res;
}