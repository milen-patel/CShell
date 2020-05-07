#ifndef NODE_H
#define NODE_H

#include "Str.h"
#include "StrVec.h"

typedef enum NodeType {
    ERROR_NODE = -1,
    COMMAND_NODE = 0,
    PIPE_NODE = 1
} NodeType;

typedef struct Node Node;

typedef const char* ErrorValue;

typedef StrVec CommandValue;

typedef struct PipeValue {
    Node *left;
    Node *right;
} PipeValue;

typedef union NodeValue {
    ErrorValue error;
    CommandValue command;
    PipeValue pipe;
} NodeValue;

struct Node {
    NodeType type;
    NodeValue data;
};

/** Node Constructorsand Destructor  */

Node* ErrorNode_new(const char *msg);

Node* CommandNode_new(StrVec words);

Node* PipeNode_new(Node *left, Node *right);

void* Node_drop(Node *self);

#endif
