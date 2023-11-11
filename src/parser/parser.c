#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

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

Parser* parser_create(char* str) {
    Parser* parser = malloc(sizeof(Parser));
    parser->scanner = scanner_create(str);
    parser->stack = TokenList_create(strlen(str) * 2, (Token){TOK_END, 0});
    parser->next = scanner_next(parser->scanner);
    return parser;
}

void parser_destroy(Parser* parser) {
    scanner_destroy(parser->scanner);
    TokenList_destroy(parser->stack);
    free(parser);
}

Token parser_next(Parser* parser) {
    Token out = (Token){TOK_NONE, '\0'};

    while (out.type == TOK_NONE) {
        switch (parser->next.type) {
            case TOK_CONCAT:
            case TOK_OR:
            case TOK_OPT:
            case TOK_STAR:
            case TOK_PLUS: {
                Token peek = TokenList_peek(parser->stack);
                if (precedence(parser->next) <= precedence(peek)) {
                    out = peek;
                    TokenList_pop(parser->stack);
                } else {
                    TokenList_push(parser->stack, parser->next);
                    parser->next = scanner_next(parser->scanner);
                }
                break;
            }
            case TOK_LBRACE:
                TokenList_push(parser->stack, parser->next);
                parser->next = scanner_next(parser->scanner);
                break;
            case TOK_RBRACE:
                if (TokenList_peek(parser->stack).type != TOK_LBRACE) {
                    out = TokenList_pop(parser->stack);
                } else {
                    TokenList_pop(parser->stack);
                    parser->next = scanner_next(parser->scanner);
                }
                break;
            case TOK_CHAR:
                out = parser->next;
                parser->next = scanner_next(parser->scanner);
                break;
            default: out = TokenList_pop(parser->stack); break;
        }
    }

    return out;
}