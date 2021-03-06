# WEPLBalancedTree

This repository contains Words16, a program that implements a version of a weighted external path length balanced tree. 
Words16 can take in textfiles to add or delete words to the tree. Words repeatedly added to the tree will have their count (number of
appearances) incremented. Moreover, you can print the (word, count) pairs in inorder traversal or dump the words in preorder.
Words16 also allows you to set an "improvement factor" or IF that determines whether a rotation should be made in the tree.

Words16
maintains a tree of (word,count) pairs and an improvement factor IF (initially
0), and accepts six types of arguments:

  TEXTFILE:        Read words from TEXTFILE; increment the count for those that
                  are in the tree; insert with a count of 1 those that are not

  -d DICTFILE:     Read words from DICTFILE; delete from the tree those that are
                  in the tree

  -print:         Write the (word,count) pairs in the tree to the standard
                  output in inorder (that is, in alphabetic order), one word
                  per line, preceded by its count, using the format "%3d %s\n"

  -dump:           Write the words (including those in internal nodes; see
                  below) in the tree to the standard output in preorder, one
                  word per line, using the format "%s".

  -epl:            Write the sum of the counts in the leaves and the weighted
                  external path length for the tree to the standard output,
                  using the format "%d, %d\n"

  -set VALUE:      Set the improvement factor IF to the integer VALUE

Each command line argument is processed at the time it is encountered.  Thus
with the command

  % Words16  Essay  -d SysDict  -dump  -set 2   Appendix  -d UsrDict  -print

the words in the file Essay will be added to the tree, the words in the file
SysDict deleted from the tree, the tree dumped in preorder, IF changed to 2
from the initial value of 0, the words in the file Appendix added to the tree,
the words in the file UsrDict deleted from the tree, and the (word,count) pairs
in the tree printed in alphabetic order.

The tree should be implemented as a WEPL-Balanced Binary Search Tree (AKA a
"2016" tree), which has some unusual properties:
                          
A) Either the tree is empty or every node has 0 or 2 children.

B) The (word,count) pairs are stored in the leaves of the tree; the words in
   the internal nodes are used only for searching.

   Thus unless the tree is empty, inserting a (word,count) pair into the tree
   creates a new internal node.  For example, inserting the pair ("FOO",1) into
   the one-node tree ("BAR",2) gives the tree

                                   "BAR"
                                  /     \
                          ("BAR",2)     ("FOO",1)

   (by Property C below, the word in the internal node must be the smaller of
   the two words).

   Similarly, deleting a (word,count) pair from the tree deletes the parent of
   that leaf and makes its sibling the child of its grandparent.  For example,
   deleting "FOO" from the tree causes

           "YUK"             "YUK"                   "FOO"             "FOO"
          /     \           /     \                 /     \           /     \
       "BAR"     T2    =>  T1      T2    OR      "BAR"     T2    =>  T1     T2
      /     \                                   /     \
     T1      ("FOO",2)                         T1      ("FOO",2)

   where T1 and T2 denote nonempty subtrees.  Note that after deletion the word
   might still exist in an internal node as in the second example above.

C) All words in the left subtree of a node (whether in leaves or in internal
   nodes) are less than or equal to the word in the node; and all words in the
   right subtree are greater than it.

D) The weight W(N) of a node N is the sum of the counts in the leaf nodes of
   the subtree rooted at N and is stored in N.  The weight W(T) of a subtree
   is the weight of its root.

E) The weighted external path length WEPL(T) of a subtree T is the weighted sum
   of the lengths of the paths from its root to each of its leaves, with the
   weights being the values of count.  WEPL(T) is stored in the root of T.

F) To improve the average search time the standard algorithms for search,
   insert, and delete are modified as follows:

   Search/Insert:
     Assume that the word W was found (and its count incremented) or inserted
     in the subtree T rooted at N.  If the leaf containing W is in the left
     [right] subtree of T, then rotate N to the right [left] whenever this
     rotation would reduce WEPL(T) by more than IF; that is, letting T' denote
     the subtree after the rotation, whenever WEPL(T') < WEPL(T) - IF.
    
   Delete:
     Assume that the word W was deleted from the subtree T rooted at N.  If the
     node that contained W is the left [right] subtree of T, then rotate N to
     the left [right] whenever this rotation would reduce WEPL(T) by more than
     IF; that is, letting T' denote the subtree after the rotation, whenever
     WEPL(T') < WEPL(T) - IF.

   Note that these rotations cannot make a leaf internal, and that the
   modifications above are applied as the recursive search/insert/delete
   unwinds.

   When IF is 0, these changes are intended to reduce the weighted external
   path length (and thus the average successful search time) whenever possible.
   (Whether these intentions are met is unknown.)
