#include <stdlib.h>

#include "token.h"

GEN_LIST_IMPL(TokenList, Token)

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
