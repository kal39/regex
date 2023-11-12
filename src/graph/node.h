#pragma once

#include "list/list.h"

typedef int NodeID;

typedef struct Node {
    char c;
    NodeID out1;
    NodeID out2;
} Node;

GEN_LIST_DEF(NodeList, Node)