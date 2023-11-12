#include <stdlib.h>
#include <string.h>

#include "scanner.h"

Scanner* scanner_create(char* str) {
    Scanner* scanner = malloc(sizeof *scanner);
    scanner->str = str;
    scanner->pos = 0;
    scanner->next = (Token){TOK_NONE, 0};
    scanner_next(scanner);
    return scanner;
}

void scanner_destroy(Scanner* scanner) {
    free(scanner);
}

Token scanner_next(Scanner* scanner) {
    if (scanner->pos > (int)strlen(scanner->str)) return (Token){TOK_END, 0};

    Token current = scanner->next;
    char c = scanner->str[scanner->pos++];

    switch (c) {
        case '\0': scanner->next = (Token){TOK_END, c}; break;
        case '?': scanner->next = (Token){TOK_OPT, c}; break;
        case '*': scanner->next = (Token){TOK_STAR, c}; break;
        case '+': scanner->next = (Token){TOK_PLUS, c}; break;
        case '|': scanner->next = (Token){TOK_OR, c}; break;
        case '(': scanner->next = (Token){TOK_LBRACE, c}; break;
        case ')': scanner->next = (Token){TOK_RBRACE, c}; break;
        default:
            if (c == '\\') c = scanner->str[scanner->pos++];
            scanner->next = (Token){TOK_CHAR, c};
            break;
    }

    if (token_is_expr_start(scanner->next) && token_is_expr_end(current)) {
        scanner->next = (Token){TOK_CONCAT, '.'};
        scanner->pos -= scanner->str[scanner->pos - 2] == '\\' ? 2 : 1;
    }

    return current;
}

Token scanner_peek(Scanner* scanner) {
    return scanner->next;
}