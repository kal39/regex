#pragma once

#include <stdbool.h>

#include "list/list.h"

typedef enum TokenType {
    TOK_CHAR,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_CONCAT,
    TOK_OR,
    TOK_OPT,
    TOK_STAR,
    TOK_PLUS,
    TOK_END,
    TOK_NONE,
} TokenType;

typedef struct Token {
    TokenType type;
    char c;
} Token;

GEN_LIST_DEF(TokenList, Token)

char* token_name(Token token);

bool token_is_expr_start(Token token);
bool token_is_expr_end(Token token);
