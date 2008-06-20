#include <stdio.h>
#include <stdlib.h>

/* The node struct that we will have in RAM */
typedef struct node{
  int leaf; /* 1 if leaf, 0 otherwize */
  int n; /* number of keys */
  int * keys; /* array of n keys (needs initialization) */
  int * pointers; /* array of n + 1 pointers to other nodes */
  int prev; /* previous node */
  int next; /* next node */
};

/* Return a pointer to a new allocated node. */
/* Also allocates the arrays inside the node. */
node * getNewNode(int n) {
  node * new_node;
  
  new_node = (node*)malloc(sizeof(node));
  
  if (new_node) {
    new_node->keys = (int*)malloc(sizeof(int) * n);
    new_node->pointers = (int*)malloc(sizeof(int) * (n + 1));
  }

  return new_node;
}

/* Return 0 if wrote ok, 1 otherwize */
int writeNode(node* no, int filename) {

}

/* Reads the info from 'filename' to no */
int readNode(node* no, int filename) {

}

/* Returns 1 if node 'no' has a underflow or overflow. */
int hasOverflowUnderflow(node* no) {

}

int main() {

  return 0;
}
