#include <stdlib.h>

#include "token.h"

// TODO: make token_list operations check len

char* token_name(Token token) {
    switch (token.type) {
        case TOK_CHAR: return "TOK_CHAR  ";
        case TOK_LBRACE: return "TOK_LBRACE";
        case TOK_RBRACE: return "TOK_RBRACE";
        case TOK_CONCAT: return "TOK_CONCAT";
        case TOK_OR: return "TOK_OR    ";
        case TOK_STAR: return "TOK_STAR  ";
        case TOK_END: return "TOK_END   ";
        default: return "TOK_NONE  ";
    }
}

bool token_is_expr_start(Token token) {
    return token.type == TOK_CHAR || token.type == TOK_LBRACE;
}

bool token_is_expr_end(Token token) {
    return token.type == TOK_CHAR || token.type == TOK_RBRACE
        || token.type == TOK_STAR;
}

TokenList* token_list_create(int len) {
    TokenList* list = malloc(sizeof *list);
    list->len = len;
    list->pos = 0;
    list->tokens = malloc(sizeof *list->tokens * len);
    return list;
}

void token_list_destroy(TokenList* list) {
    free(list->tokens);
    free(list);
}

Token token_list_peek(TokenList* list) {
    return list->pos > 0 ? list->tokens[list->pos - 1] : (Token){TOK_END, '\0'};
}

Token token_list_pop(TokenList* list) {
    return list->pos > 0 ? list->tokens[--list->pos] : (Token){TOK_END, '\0'};
}

void token_list_push(TokenList* list, Token token) {
    if (list->pos < list->len) list->tokens[list->pos++] = token;
}