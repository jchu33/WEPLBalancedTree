// Tree.c
// Jason Chu
// An implementation of the WEPL-balanced binary search tree. Has functions to create trees, makeNodes, rotateRight
// and rotateLeft if tree needs to be balanced again, insert and delete, and print out the trees in inorder
// and preorder.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// Internal representation of a Tree is a pointer to a tnode
// type definition of a tnode in a tree
// Field Key key is a pointer to the "key" stored in the tnode
// Field int count is the count (number of appearances for leaves and weight for internal nodes) of the tnode
// Field int wepl is the WEPL value for the tnode
// Field Tree left is the pointer to the tnode on the left side of the tree
// Field Tree right is the pointer to the tnode on the right side of the tree
struct tnode {
    Key key;                      // Each Tree node contains a KEY and
    int count;
    int wepl;
    Tree left, right;             // ... has a LEFT and a RIGHT subtree
};


// Set *T to the empty tree.  Return TRUE if successful, else FALSE.
bool create(Tree *t)
{
    *t = NULL;
    return true;
}

// print out the count and WEPL of the given Tree
void epl(Tree *t)
{
    if (*t == NULL)
    {
        return;
    }
    else
    {
        printf("%d, %d\n", (*t)->count, (*t)->wepl);
    }
}

// Set *T to a one-node tree containing Key K.
// Sets count to passed in count, wepl to passed in wepl
void makeNode(Tree *t, Key k, int count, int wepl)
{
    struct tnode *temp = malloc(sizeof(struct tnode));
    (*temp).key = k;
    (*temp).count = count;
    (*temp).wepl = wepl;
    (*temp).left = NULL;
    (*temp).right = NULL;
    (*t) = temp;
}

// Manipulates pointers to rotate the tree (single rotation) to the right
void rotateRight(Tree *t)
{
    if ((*t)->left->right != NULL)
    {
        // temp stores old parent's left child which is new parent
        Tree temp = (*t)->left;
        (*t)->left = (*t)->left->right;
        (*t)->count = (*t)->left->count + (*t)->right->count;
        (*t)->wepl = (*t)->left->count + (*t)->left->wepl + \
            (*t)->right->count + (*t)->right->wepl;
        // new parent's right child is old parent
        temp->right = (*t);
        temp->count = temp->left->count + temp->right->count;
        temp->wepl = temp->left->count + temp->left->wepl+ \
            temp->right->count + temp->right->wepl;
        (*t) = temp;
    }
}

// Manipulates pointers to rotate the tree (single rotation) to the left
void rotateLeft(Tree *t)
{
    if ((*t)->right->left != NULL)
    {
        // temp stores old parent's right child which is new parent
        Tree temp = (*t)->right;
        (*t)->right = (*t)->right->left;
        (*t)->count = (*t)->left->count + (*t)->right->count;
        (*t)->wepl = (*t)->left->count + (*t)->left->wepl + \
            (*t)->right->count + (*t)->right->wepl;
        // new parent's left child is old parent
        temp->left = (*t);
        temp->count = temp->left->count + temp->right->count;
        temp->wepl = temp->left->count + temp->left->wepl + \
            temp->right->count + temp->right->wepl;
        (*t) = temp;
    }
}

// returns true if the passed in tree is a leaf (no left/right children)
bool isLeaf(Tree *t)
{
    if (*t != NULL && (*t)->left == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Search for Key K in Tree *T and increment count in that leaf if found.
// Recursive function that compares keys to current key and determine if insert in left/right subtree
// If insertion satisfies WEPL(T') < WEPL(T) - IF, where T' is the rotated tree, then do a rotation
void insert(Tree *t, Key k, int improvFactor)
{
    // if the tree is empty, just makea node
    if (*t == NULL)
    {
        char *str = malloc((strlen(k) + 1)*sizeof(char));
        strcpy(str, k);
        makeNode(t, str, 1, 0);
    }
    // if tree is a leaf, then split the leaf and create a new tree if key not found
    else if (isLeaf(t))
    {
        // if key found, just increment count
        if (strcmp(k, (*t)->key) == 0)
        {
            (*t)->count = (*t)->count + 1;
        }
        // if key k less than the leaf's key, new tree with parent having key k
        else if (strcmp(k, (*t)->key) < 0)
        {
            char *tkey = malloc((strlen((*t)->key) + 1)*sizeof(char));
            strcpy(tkey, (*t)->key);
            char *str = malloc((strlen(k) + 1)*sizeof(char));
            strcpy(str, k);
            makeNode(&(*t)->right, tkey, (*t)->count, 0);
            makeNode(&(*t)->left, str, 1, 0);
            char *str2 = malloc((strlen(k) + 1)*sizeof(char));
            strcpy(str2, k);
            free((*t)->key);
            (*t)->key = str2;
            (*t)->count = (*t)->count + 1;
            (*t)->wepl = (*t)->left->count + (*t)->right->count;
        }
        // if key k greater than the leaf's key, new tree with parent having leaf's key
        else if (strcmp(k, (*t)->key) > 0)
        {
            char *str = malloc((strlen(k) + 1)*sizeof(char));
            strcpy(str, k);
            char *tkey = malloc((strlen((*t)->key) + 1)*sizeof(char));
            strcpy(tkey, (*t)->key);
            makeNode(&(*t)->left, tkey, (*t)->count, 0);
            makeNode(&(*t)->right, str, 1, 0);
            (*t)->count = (*t)->count + 1;
            (*t)->wepl = (*t)->left->count + (*t)->right->count;
        }
        return;
    }
    // if internal node, recursively call insert
    else
    {
        bool insertLeft = false;
        if (strcmp(k, (*t)->key) <= 0)
        {
            insertLeft = true;
            (*t)->count = (*t)->count + 1;
            insert(&(*t)->left, k, improvFactor);
        }
        else if (strcmp(k, (*t)->key) > 0)
        {
            insertLeft = false;
            (*t)->count = (*t)->count + 1;
            insert(&(*t)->right, k, improvFactor);
        }
        // update WEPL
        (*t)->wepl = (*t)->left->count + (*t)->left->wepl + \
            (*t)->right->count + (*t)->right->wepl;
        
        if (insertLeft && (*t)->left->left != NULL)
        {
            // equation to determine if rotation is needed
            if ((*t)->left->left->count > (*t)->right->count + improvFactor)
            {
                rotateRight(t);
            }
        }
        if (!insertLeft && (*t)->right->right != NULL)
        {
            // equation to determine if rotation is needed
            if ((*t)->right->right->count > (*t)->left->count + improvFactor)
            {
                rotateLeft(t);
            }
        }
    }
}


// Search for Key K in Tree *T and delete that leaf if found
// Recursive function that compares keys to current key and determine if delete in left/right subtree
// If deletion satisfies WEPL(T') < WEPL(T) - IF, where T' is the rotated tree, then do a rotation
// returns true if deletion was successful, false if not
bool delete(Tree *t, Key k, int improvFactor)
{
    if (*t == NULL)
    {
        return false;
    }
    // if t is a leaf awith key k, just free that node
    else if (isLeaf(t) && strcmp(k, (*t)->key) == 0)
    {
        free((*t)->key);
        free(*t);
        *t = NULL;
        return true;
    }
    // delete right child and free parent and right child
    else if (isLeaf(&(*t)->right) && strcmp(k, (*t)->right->key) == 0)
    {
        Tree parent = *t;
        Tree right = (*t)->right;
        *t = (*t)->left;
        free(parent->key);
        free(parent);
        free(right->key);
        free(right);
        return true;
    }
    // delete left child and free parent and left child
    else if (isLeaf(&(*t)->left) && strcmp(k, (*t)->left->key) == 0)
    {
        Tree parent = *t;
        Tree left = (*t)->left;
        *t = (*t)->right;
        free(parent->key);
        free(parent);
        free(left->key);
        free(left);
        return true;
    }
    // if internal node, call delete recursively
    else if (!isLeaf(t) && strcmp(k, (*t)->key) <= 0)
    {
        if (delete(&(*t)->left, k, improvFactor))
         {
             // equation to determine if rotation needed
             if ((*t)->right->right != NULL && (*t)->right->right->count > \
                 (*t)->left->count + improvFactor)
             {
                 rotateLeft(t);
             }
             return true;
         }
        else
            return false;
    }
    else if (!isLeaf(t) && strcmp(k, (*t)->key) > 0)
    {
        if (delete(&(*t)->right, k, improvFactor))
        {
            if ((*t)->left->left != NULL && (*t)->left->left->count > \
                (*t)->right->count + improvFactor)
            {
                rotateRight(t);
            }
            return true;
        }
        else
            return false;
    }
    else
    {
        return false;
    }
}

// visitInOrder prints out the count, key pair for the current leaf
void visitInOrder(Tree t)
{
    if (t != NULL && isLeaf(&t))
    {
        printf("%3d %s\n", t->count, t->key);
    }
}

// visitPreOrder prints out the count, key pair for the current leaf
void visitPreOrder(Tree t)
{
    if (t != NULL)
    {
        printf("%s\n", t->key);
    }
}

// Perform an inorder traversal of Tree T, printing one count, key pair per line
void inorder(Tree t)
{
    if (t == NULL)
    {
        return;
    }
    inorder(t->left);
    visitInOrder(t);
    inorder(t->right);
}


// Perform a preorder traversal of Tree T, printing one Key per line.
void preorder(Tree t)
{
    if (t == NULL)
    {
        return;
    }
    visitPreOrder(t);
    preorder(t->left);
    preorder(t->right);
}

