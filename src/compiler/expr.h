#pragma once

#include "graph.h"
#include "list/list.h"

typedef struct Expr {
    Node* start;
    Node* end;
} Expr;

GEN_LIST_DEF(ExprList, Expr)
