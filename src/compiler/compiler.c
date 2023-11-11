// ref: https://swtch.com/~rsc/regexp/regexp1.html

#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "expr.h"
#include "parser/parser.h"

Graph* compile(char* regex) {
    Parser* p = parser_create(regex);
    ExprList* stack = ExprList_create(strlen(regex) * 2, (Expr){NULL, NULL});
    Graph* g = graph_create();

    for (Token t = parser_next(p); t.type != TOK_END; t = parser_next(p)) {
        switch (t.type) {
            case TOK_CHAR: {
                Node* node = graph_new_node(g, t.c, NULL, NULL);
                ExprList_push(stack, (Expr){node, node});
                break;
            }
            case TOK_CONCAT: {
                Expr e2 = ExprList_pop(stack);
                Expr e1 = ExprList_pop(stack);
                e1.end->out1 = e2.start;
                ExprList_push(stack, (Expr){e1.start, e2.end});
                break;
            }
            case TOK_OR: {
                Expr e1 = ExprList_pop(stack);
                Expr e2 = ExprList_pop(stack);

                Node* start = graph_new_node(g, 0, e1.start, e2.start);
                Node* end = graph_new_node(g, 0, NULL, NULL);
                e1.end->out1 = end;
                e2.end->out1 = end;

                ExprList_push(stack, (Expr){start, end});
                break;
            }
            case TOK_OPT: {
                Expr e = ExprList_pop(stack);

                Node* end = graph_new_node(g, 0, NULL, NULL);
                Node* start = graph_new_node(g, 0, e.start, end);
                e.end->out1 = end;

                ExprList_push(stack, (Expr){start, end});
                break;
            }
            case TOK_STAR: {
                Expr e = ExprList_pop(stack);

                Node* end = graph_new_node(g, 0, NULL, NULL);
                Node* start = graph_new_node(g, 0, e.start, end);
                e.end->out1 = start;

                ExprList_push(stack, (Expr){start, end});
                break;
            }
            case TOK_PLUS: {
                Expr e = ExprList_pop(stack);

                Node* end = graph_new_node(g, 0, NULL, NULL);
                Node* mid = graph_new_node(g, 0, end, e.start);
                e.end->out1 = mid;

                ExprList_push(stack, (Expr){e.start, end});
                break;
            }

            default: break;
        }
    }

    parser_destroy(p);

    graph_set_start(g, ExprList_peek(stack).start);
    graph_set_end(g, ExprList_peek(stack).end);

    ExprList_destroy(stack);

    return g;
}