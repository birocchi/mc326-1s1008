#include <stdio.h>
#include <stdlib.h>

/* The node struct that we will have in RAM */
typedef struct {
  int leaf; /* 1 if leaf, 0 otherwize */
  int n; /* number of keys */
  int * keys; /* array of n keys (needs initialization) */
  int * pointers; /* array of n + 1 pointers to other nodes */
  int prev; /* previous node */
  int next; /* next node */
} Node;

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
int writeNode(Node* node, int filename) {

}

/* Reads the info from 'filename' to node */
int readNode(Node* node, int filename) {

}

/* Returns 1 if Node 'node' has a underflow or overflow. */
int hasOverflowUnderflow(Node* node) {

}

/* Returns the key after which the pointer to the next node is  */
/* to find key */
int findKey(Node * node, int key) {
  int i;
  
  for (i = 0; i < node->n; i++) {
    if (node->keys[i] > key) {
      break;
    }
  }
  return i;
}

int main() {

  return 0;
}
