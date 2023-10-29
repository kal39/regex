#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser/parser.h"

int main() {
    // int main(int argc, char* argv[]) {
    // if (argc != 3) {
    //     printf("usage: %s [string-to-match] [regex]\n", argv[0]);
    //     return -1;
    // }

    // char* str = argv[1];
    // char* regex_str = argv[2];

    char* regex_str = "a(b|c)*d";

    printf("input: %s\n", regex_str);

    Parser* p = parser_create(regex_str);

    printf("parse: ");
    for (Token t = parser_next(p); t.type != TOK_END; t = parser_next(p))
        printf(t.type == TOK_CHAR ? "%c" : "%c", t.c);
    printf("\n");

    parser_destroy(p);

    return 0;
}