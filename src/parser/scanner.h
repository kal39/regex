#pragma once

#include "token.h"

typedef struct Scanner {
    char* str;
    int pos;
    Token next;
} Scanner;

Scanner* scanner_create(char* str);
void scanner_destroy(Scanner* scanner);

Token scanner_next(Scanner* scanner);
Token scanner_peek(Scanner* scanner);
