#include <stdlib.h>
#include <string.h>

#include "scanner.h"

Scanner* scanner_create(char* str) {
    Scanner* scanner = malloc(sizeof *scanner);
    scanner->str = str;
    scanner->pos = 0;
    scanner->prev = (Token){TOK_END, '\0'};
    return scanner;
}

void scanner_destroy(Scanner* scanner) {
    free(scanner);
}

Token scanner_next(Scanner* scanner) {
    if (scanner->pos >= (int)strlen(scanner->str))
        return (Token){TOK_END, '\0'};

    char c = scanner->str[scanner->pos++];
    Token next;

    switch (c) {
        case '\0': next = (Token){TOK_END, c}; break;
        case '?': next = (Token){TOK_OPT, c}; break;
        case '*': next = (Token){TOK_STAR, c}; break;
        case '+': next = (Token){TOK_PLUS, c}; break;
        case '|': next = (Token){TOK_OR, c}; break;
        case '(': next = (Token){TOK_LBRACE, c}; break;
        case ')': next = (Token){TOK_RBRACE, c}; break;
        default:
            if (c == '\\') c = scanner->str[scanner->pos++];
            next = (Token){TOK_CHAR, c};
            break;
    }

    if (token_is_expr_start(next) && token_is_expr_end(scanner->prev)) {
        scanner->prev = (Token){TOK_CONCAT, '.'};
        scanner->pos -= scanner->str[scanner->pos - 2] == '\\' ? 2 : 1;
    } else {
        scanner->prev = next;
    }

    return scanner->prev;
}