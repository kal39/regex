#pragma once

#include "token.h"

typedef struct Scanner {
    char* str;
    int pos;
    Token prev;
} Scanner;

char* token_type_to_str(TokenType type);

Scanner* scanner_create(char* str);

void scanner_destroy(Scanner* scanner);

Token scanner_next(Scanner* scanner);
