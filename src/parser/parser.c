// ref: https://swtch.com/~rsc/regexp/regexp1.html
// ref: https://mathcenter.oxford.emory.edu/site/cs171/shuntingYardAlgorithm/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "scanner.h"

typedef struct Expr {
    NodeID start;
    NodeID end;
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
            NodeID node = graph_add(g, t.c, 0, 0);
            ExprList_push(eStack, (Expr){node, node});
            break;
        }
        case TOK_CONCAT: {
            Expr e2 = ExprList_pop(eStack);
            Expr e1 = ExprList_pop(eStack);
            graph_get(g, e1.end)->out1 = e2.start;
            ExprList_push(eStack, (Expr){e1.start, e2.end});
            break;
        }
        case TOK_OR: {
            Expr e1 = ExprList_pop(eStack);
            Expr e2 = ExprList_pop(eStack);
            NodeID start = graph_add(g, character_empty(), e1.start, e2.start);
            NodeID end = graph_add(g, character_empty(), 0, 0);
            graph_get(g, e1.end)->out2 = end;
            graph_get(g, e2.end)->out1 = end;
            ExprList_push(eStack, (Expr){start, end});
            break;
        }
        case TOK_OPT: {
            Expr e = ExprList_pop(eStack);
            NodeID end = graph_add(g, character_empty(), 0, 0);
            NodeID start = graph_add(g, character_empty(), e.start, end);
            graph_get(g, e.end)->out1 = end;
            ExprList_push(eStack, (Expr){start, end});
            break;
        }
        case TOK_STAR: {
            Expr e = ExprList_pop(eStack);
            NodeID end = graph_add(g, character_empty(), 0, 0);
            NodeID start = graph_add(g, character_empty(), e.start, end);
            graph_get(g, e.end)->out1 = start;
            ExprList_push(eStack, (Expr){start, end});
            break;
        }
        case TOK_PLUS: {
            Expr e = ExprList_pop(eStack);
            NodeID end = graph_add(g, character_empty(), 0, 0);
            NodeID mid = graph_add(g, character_empty(), end, e.start);
            graph_get(g, e.end)->out1 = mid;
            ExprList_push(eStack, (Expr){e.start, end});
            break;
        }

        default: break;
    }
}

Graph* parse(char* regex) {
    Scanner* s = scanner_create(regex);
    TokenList* tStack
        = TokenList_create(0, (Token){TOK_END, character_empty()});
    ExprList* eStack = ExprList_create(0, (Expr){0, 0});
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

    g->start = ExprList_peek(eStack).start;
    g->end = ExprList_peek(eStack).end;

    scanner_destroy(s);
    TokenList_destroy(tStack);
    ExprList_destroy(eStack);

    return g;
}