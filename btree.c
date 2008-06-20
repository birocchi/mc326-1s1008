#include <stdio.h>
#include <stdlib.h>

#define TAM 4

/* The node struct that we will have in RAM */
typedef struct {
  int leaf; /* 1 if leaf, 0 otherwize */
  int n; /* max number of keys */
  int used; /* number of keys already in use */
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
    new_node->n = n;
    /* we allocate one spare position so we can deal with overflows later */
    new_node->keys = (int*)malloc(sizeof(int) * (n + 1));
    new_node->pointers = (int*)malloc(sizeof(int) * (n + 2));
    new_node->used = 0;
    new_node->prev = -1;
    new_node->next = -1;
  }

  int i;
  for (i = 0; i < n + 2; i++) {
    new_node->pointers[i] = -1;
  }

  return new_node;
}

/* Return 0 if wrote ok, 1 otherwize */
int writeNode(Node* node, int filename) {

  return 0;
}

/* Reads the info from 'filename' to node */
int readNode(Node* node, int filename) {

  return 0;
}

/* Returns 1 if Node 'node' has a underflow or overflow. */
int hasOverflowUnderflow(Node* node) {
  
  /* overflow */
  if (node->used > node->n) {
    return 1;
  }
  /* underflow */
  if (node->used < (node->n) / 2) {
    return 1;
  }
  
  return 0;
}

/* Returns the key after which the pointer to the next node is  */
/* to find key */
int findKey(Node * node, int key) {
  int i = 0;
  
  if (node->used != 0) {

    for (i = 0; i < node->n; i++) {
      if (node->keys[i] > key) {
	break;
      }
    }
    
  }
  return i;
}

/* Sorts the node by key */
int sortNode (Node * node) {

  return 0;
}

int insertData(Node * node, int key) {
  
  if (node->used <= node->n) {
    node->keys[node->used] = key;
  }
  else {
    /* Split */
  }
  sortNode(node);

  return 0;
}

void printNode(Node * node) {

  int i;

  printf("Leaf: %d\n", node->leaf);
  printf("Max number of keys: %d\n", node->n);
  printf("Number of keys used: %d\n", node->used);
  printf("Pointer to previous node: %d\n", node->prev);
  printf("Pointer to next node: %d\n", node->next);

  printf("Keys:\n  -->");
  for (i = 0; i < node->used; i++) {
    printf(" %d", node->keys[i]);
  }
  printf("\n");

  printf("Pointers:\n  -->");
  for (i = 0; i < node->n + 2; i++) {
    printf(" %d", node->pointers[i]);
  }
  printf("\n");

  return;
}

/* Print the node's keys */
void printKeys(Node * node) {
  int i;

  for (i = 0; i < node->used; i++) {
    printf("%d ", node->keys[i]);
  }
  printf("\n");
  
  return;
}

int main() {
  
  Node * root = getNewNode(TAM);
  root->leaf = 0;
  
  int key;

  while (1) {
    scanf("%d", &key);
    printNode(root);
  }
  
  return 0;
}
