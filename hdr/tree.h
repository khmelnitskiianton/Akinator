#ifndef TREE_H
#define TREE_H

#include "element.h"

struct Node {
    Elem_t       Value[SIZE_OF_VALUE];
    struct Node* Right;
    struct Node* Left;
    struct Node* Parent;
};
 
typedef struct Node Node_t;

typedef struct BinaryTree {
    Node_t* Root;
    size_t  Size;
} BinaryTree_t;

#endif