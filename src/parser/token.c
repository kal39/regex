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
        case TOK_OPT: return "TOK_OPT   ";
        case TOK_STAR: return "TOK_STAR  ";
        case TOK_PLUS: return "TOK_PLUS  ";
        case TOK_END: return "TOK_END   ";
        default: return "TOK_NONE  ";
    }
}

bool token_is_expr_start(Token token) {
    switch (token.type) {
        case TOK_CHAR: return true;
        case TOK_LBRACE: return true;
        case TOK_RBRACE: return false;
        case TOK_CONCAT: return false;
        case TOK_OR: return false;
        case TOK_OPT: return false;
        case TOK_STAR: return false;
        case TOK_PLUS: return false;
        case TOK_END: return false;
        default: return false;
    }
}

bool token_is_expr_end(Token token) {
    switch (token.type) {
        case TOK_CHAR: return true;
        case TOK_LBRACE: return false;
        case TOK_RBRACE: return true;
        case TOK_CONCAT: return false;
        case TOK_OR: return false;
        case TOK_OPT: return true;
        case TOK_STAR: return true;
        case TOK_PLUS: return true;
        case TOK_END: return false;
        default: return false;
    }
}
