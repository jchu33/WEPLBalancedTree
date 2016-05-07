// Tree.h                                      Jason Chu 4/21/2016
//
// A possible interface specification for a WEPL-Balanced Binary Search Tree

#include <stdbool.h>

// External definition of Tree
typedef struct tnode *Tree;

// Datatype for Key
typedef char *Key;

// Set *T to the empty tree.  Return TRUE if successful, else FALSE.
bool create(Tree *t);

// Set *T to a one-node tree containing Key k with count/WEPLs passed in
void makeNode(Tree *t, Key k, int count, int wepl);

// print out the count and WEPL of the given Tree
void epl(Tree *t);

// Search for Key K in Tree *T and insert K in *T if not found; perform rotations if necessary to restore
// WEPL balance.
void insert(Tree *t, Key k, int improvFactor);

// Delete Key K from Tree *T.  Return TRUE if successful, else FALSE.
// Perform rotations if necessary to restore WEPL balance
bool delete(Tree *t, Key k, int improvFactor);

// Perform an inorder traversal of Tree T, printing one Key per line.
void inorder(Tree t);

// Perform a preorder traversal of Tree T, printing one Key per line.
void preorder(Tree t);

