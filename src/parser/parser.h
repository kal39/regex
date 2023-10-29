// ref: https://aquarchitect.github.io/swift-algorithm-club/Shunting%20Yard/

#pragma once

#include "scanner.h"

typedef struct Parser {
    Scanner* scanner;
    TokenList* stack;
    Token next;
} Parser;

Parser* parser_create(char* str);
void parser_destroy(Parser* parser);

Token parser_next(Parser* parser);
