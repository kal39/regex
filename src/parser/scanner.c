#include <stdlib.h>
#include <string.h>

#include "scanner.h"

static void parse_character(Scanner* s, char c) {
    if (s->str[s->pos] == '-') {
        s->pos += 2;
        s->n = (Token){
            TOK_CHAR,
            character_range(c, s->str[s->pos - 1]),
        };
    } else {
        s->n = (Token){TOK_CHAR, character(c)};
    }
}

Scanner* scanner_create(char* str) {
    Scanner* s = malloc(sizeof *s);
    s->str = str;
    s->pos = 0;
    s->n = (Token){TOK_NONE, character_empty()};
    s->nConcat = false;
    scanner_next(s);
    return s;
}

void scanner_destroy(Scanner* s) {
    free(s);
}

Token scanner_next(Scanner* s) {
    if (s->pos > (int)strlen(s->str))
        return (Token){TOK_END, character_empty()};

    if (s->nConcat) {
        s->nConcat = false;
        return (Token){TOK_CONCAT, character_empty()};
    }

    Token current = s->n;
    char c = s->str[s->pos++];

    switch (c) {
        case '\0': s->n = (Token){TOK_END, character(c)}; break;
        case '?': s->n = (Token){TOK_OPT, character(c)}; break;
        case '*': s->n = (Token){TOK_STAR, character(c)}; break;
        case '+': s->n = (Token){TOK_PLUS, character(c)}; break;
        case '|': s->n = (Token){TOK_OR, character(c)}; break;
        case '(': s->n = (Token){TOK_LBRACE, character(c)}; break;
        case ')': s->n = (Token){TOK_RBRACE, character(c)}; break;
        case '^':
            c = s->str[s->pos++];
            parse_character(s, c);
            s->n.c.type = CHAR_COMPLEMENT;
            break;
        case '\\':
            c = s->str[s->pos++];
            switch (c) {
                case 'n': s->n = (Token){TOK_CHAR, character('\n')}; break;
                case 'r': s->n = (Token){TOK_CHAR, character('\r')}; break;
                case 't': s->n = (Token){TOK_CHAR, character('\t')}; break;
                default: parse_character(s, c); break;
            }
            break;
        default: parse_character(s, c); break;
    }

    if (token_is_expr_start(s->n) && token_is_expr_end(current))
        s->nConcat = true;

    return current;
}

Token scanner_peek(Scanner* s) {
    return s->n;
}