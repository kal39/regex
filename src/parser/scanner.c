#include <stdlib.h>
#include <string.h>

#include "scanner.h"

static void parse_character(Scanner* scanner, char c) {
    if (scanner->str[scanner->pos] == '-') {
        scanner->pos += 2;
        scanner->next = (Token){
            TOK_CHAR,
            character_range(c, scanner->str[scanner->pos - 1]),
        };
    } else {
        scanner->next = (Token){TOK_CHAR, character(c)};
    }
}

Scanner* scanner_create(char* str) {
    Scanner* scanner = malloc(sizeof *scanner);
    scanner->str = str;
    scanner->pos = 0;
    scanner->next = (Token){TOK_NONE, character_empty()};
    scanner->nextIsConcat = false;
    scanner_next(scanner);
    return scanner;
}

void scanner_destroy(Scanner* scanner) {
    free(scanner);
}

Token scanner_next(Scanner* scanner) {
    if (scanner->pos > (int)strlen(scanner->str))
        return (Token){TOK_END, character_empty()};

    if (scanner->nextIsConcat) {
        scanner->nextIsConcat = false;
        return (Token){TOK_CONCAT, character_empty()};
    }

    Token current = scanner->next;
    char c = scanner->str[scanner->pos++];

    switch (c) {
        case '\0': scanner->next = (Token){TOK_END, character(c)}; break;
        case '?': scanner->next = (Token){TOK_OPT, character(c)}; break;
        case '*': scanner->next = (Token){TOK_STAR, character(c)}; break;
        case '+': scanner->next = (Token){TOK_PLUS, character(c)}; break;
        case '|': scanner->next = (Token){TOK_OR, character(c)}; break;
        case '(': scanner->next = (Token){TOK_LBRACE, character(c)}; break;
        case ')': scanner->next = (Token){TOK_RBRACE, character(c)}; break;
        case '^':
            c = scanner->str[scanner->pos++];
            parse_character(scanner, c);
            scanner->next.c.type = CHAR_COMPLEMENT;
            break;
        case '\\':
            c = scanner->str[scanner->pos++];
            switch (c) {
                case 'n':
                    scanner->next = (Token){TOK_CHAR, character('\n')};
                    break;
                case 'r':
                    scanner->next = (Token){TOK_CHAR, character('\r')};
                    break;
                case 't':
                    scanner->next = (Token){TOK_CHAR, character('\t')};
                    break;
                case 'w':
                    scanner->next
                        = (Token){TOK_CHAR, character_range('a', 'z')};
                    break;
                case 'W':
                    scanner->next
                        = (Token){TOK_CHAR, character_complement('a', 'z')};
                    break;
                case 'd':
                    scanner->next
                        = (Token){TOK_CHAR, character_range('0', '9')};
                    break;
                case 'D':
                    scanner->next
                        = (Token){TOK_CHAR, character_complement('0', '9')};
                    break;
                default: parse_character(scanner, c); break;
            }
            break;
        default: parse_character(scanner, c); break;
    }

    if (token_is_expr_start(scanner->next) && token_is_expr_end(current))
        scanner->nextIsConcat = true;

    return current;
}

Token scanner_peek(Scanner* scanner) {
    return scanner->next;
}