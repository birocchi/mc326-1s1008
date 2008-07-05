#include "bplustree.h"

/* Return a pointer to a new allocated B+Tree. */
B+Tree * getNewB+Tree(){
  B+Tree new_B+tree;
  
  new_B+tree = (B+Tree*)malloc(sizeof(B+Tree));
  
  new_B+tree->root = 0;  /* The id of the root is '0' */
  new_B+tree->numreg = 0; /* There are no registers yet */

  return new_B+tree;
}


/* Return a pointer to a new allocated Node. */
/* Also allocates the arrays inside the Node. */
Node * getNewNode(int n) {
  Node * new_node;
  
  new_node = (Node*)malloc(sizeof(Node));
  
  if (new_node) {
    new_node->keys = (int*)malloc(sizeof(int) * n);
    new_node->pointers = (int*)malloc(sizeof(int) * (n + 1));
  }

  return new_node;
}

/* Return 0 if wrote ok, 1 otherwize */
int writeNode(Node* no, int filename) {

}

/* Reads the info from 'filename' to no */
int readNode(Node* no, int filename) {

}

/* Returns 1 if Node 'no' has a underflow or overflow. */
int hasOverflowUnderflow(Node* no) {

}


