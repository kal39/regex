#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static int precedence(Token token) {
    switch (token.type) {
        case TOK_STAR: return 3;
        case TOK_CONCAT: return 2;
        case TOK_OR: return 1;
        default: return 0;
    }
}

Parser* parser_create(char* str) {
    Parser* parser = malloc(sizeof(Parser));
    parser->scanner = scanner_create(str);
    parser->stack = token_list_create(strlen(str));
    parser->next = scanner_next(parser->scanner);
    return parser;
}

void parser_destroy(Parser* parser) {
    scanner_destroy(parser->scanner);
    token_list_destroy(parser->stack);
    free(parser);
}

Token parser_next(Parser* parser) {
    Token out = (Token){TOK_NONE, '\0'};

    while (out.type == TOK_NONE) {
        //     printf("next: '%c', out: '%c'\n", parser->next.c, out.c);
        //     getchar();

        switch (parser->next.type) {
            case TOK_CONCAT:
            case TOK_OR:
            case TOK_STAR: {
                Token peek = token_list_peek(parser->stack);
                if (precedence(parser->next) <= precedence(peek)) {
                    out = peek;
                    token_list_pop(parser->stack);
                } else {
                    token_list_push(parser->stack, parser->next);
                    parser->next = scanner_next(parser->scanner);
                }
                break;
            }
            case TOK_LBRACE:
                token_list_push(parser->stack, parser->next);
                parser->next = scanner_next(parser->scanner);
                break;
            case TOK_RBRACE:
                if (token_list_peek(parser->stack).type != TOK_LBRACE) {
                    out = token_list_pop(parser->stack);
                } else {
                    token_list_pop(parser->stack);
                    parser->next = scanner_next(parser->scanner);
                }
                break;
            case TOK_CHAR:
                out = parser->next;
                parser->next = scanner_next(parser->scanner);
                break;
            default: out = token_list_pop(parser->stack); break;
        }
    }

    return out;
}