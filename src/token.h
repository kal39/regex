#pragma once

#include <stdbool.h>

typedef enum TokenType {
    TOK_CHAR,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_CONCAT,
    TOK_OR,
    TOK_STAR,
    TOK_END,
    TOK_NONE,
} TokenType;

typedef struct Token {
    TokenType type;
    char c;
} Token;

typedef struct TokenList {
    int len;
    int pos;
    Token* tokens;
} TokenList;

char* token_name(Token token);

bool token_is_expr_start(Token token);
bool token_is_expr_end(Token token);

int token_precedence(Token token);

TokenList* token_list_create(int len);
void token_list_destroy(TokenList* list);

Token token_list_peek(TokenList* list);
Token token_list_pop(TokenList* list);
void token_list_push(TokenList* list, Token token);
