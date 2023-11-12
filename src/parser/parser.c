// ref: https://swtch.com/~rsc/regexp/regexp1.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "scanner.h"

typedef struct Expr {
    Node* start;
    Node* end;
} Expr;

GEN_LIST_DEF(ExprList, Expr)
GEN_LIST_IMPL(ExprList, Expr)

static int precedence(Token token) {
    switch (token.type) {
        case TOK_OPT:
        case TOK_STAR:
        case TOK_PLUS: return 3;
        case TOK_CONCAT: return 2;
        case TOK_OR: return 1;
        default: return 0;
    }
}

static void add_token(Graph* g, ExprList* eStack, Token t) {
    switch (t.type) {
        case TOK_CHAR: {
            Node* node = graph_new_node(g, t.c, NULL, NULL);
            ExprList_push(eStack, (Expr){node, node});
            break;
        }
        case TOK_CONCAT: {
            Expr e2 = ExprList_pop(eStack);
            Expr e1 = ExprList_pop(eStack);
            e1.end->out1 = e2.start;
            ExprList_push(eStack, (Expr){e1.start, e2.end});
            break;
        }
        case TOK_OR: {
            Expr e1 = ExprList_pop(eStack);
            Expr e2 = ExprList_pop(eStack);

            Node* start = graph_new_node(g, 0, e1.start, e2.start);
            Node* end = graph_new_node(g, 0, NULL, NULL);
            e1.end->out1 = end;
            e2.end->out1 = end;

            ExprList_push(eStack, (Expr){start, end});
            break;
        }
        case TOK_OPT: {
            Expr e = ExprList_pop(eStack);

            Node* end = graph_new_node(g, 0, NULL, NULL);
            Node* start = graph_new_node(g, 0, e.start, end);
            e.end->out1 = end;

            ExprList_push(eStack, (Expr){start, end});
            break;
        }
        case TOK_STAR: {
            Expr e = ExprList_pop(eStack);

            Node* end = graph_new_node(g, 0, NULL, NULL);
            Node* start = graph_new_node(g, 0, e.start, end);
            e.end->out1 = start;

            ExprList_push(eStack, (Expr){start, end});
            break;
        }
        case TOK_PLUS: {
            Expr e = ExprList_pop(eStack);

            Node* end = graph_new_node(g, 0, NULL, NULL);
            Node* mid = graph_new_node(g, 0, end, e.start);
            e.end->out1 = mid;

            ExprList_push(eStack, (Expr){e.start, end});
            break;
        }

        default: break;
    }
}

Graph* parse(char* regex) {
    Scanner* s = scanner_create(regex);
    TokenList* tStack = TokenList_create(0, (Token){TOK_END, 0});
    ExprList* eStack = ExprList_create(0, (Expr){NULL, NULL});
    Graph* g = graph_create();

    for (bool cont = true; cont;) {
        Token t = scanner_next(s);

        switch (t.type) {
            case TOK_CONCAT:
            case TOK_OR:
            case TOK_OPT:
            case TOK_STAR:
            case TOK_PLUS: {
                while (precedence(t) <= precedence(TokenList_peek(tStack)))
                    add_token(g, eStack, TokenList_pop(tStack));
                TokenList_push(tStack, t);
                break;
            }
            case TOK_LBRACE: {
                TokenList_push(tStack, t);
                break;
            }
            case TOK_RBRACE: {
                while (TokenList_peek(tStack).type != TOK_LBRACE)
                    add_token(g, eStack, TokenList_pop(tStack));
                TokenList_pop(tStack);
                break;
            }
            case TOK_CHAR: {
                add_token(g, eStack, t);
                break;
            }
            default: {
                while (TokenList_len(tStack) > 0)
                    add_token(g, eStack, TokenList_pop(tStack));
                cont = false;
                break;
            }
        }
    }

    graph_set_start(g, ExprList_peek(eStack).start);
    graph_set_end(g, ExprList_peek(eStack).end);

    scanner_destroy(s);
    TokenList_destroy(tStack);
    ExprList_destroy(eStack);

    return g;
}