
/* The struct that represents the B+ Tree*/
typedef struct{
  int root;    /* The id of the root of the Tree*/
  int numreg;  /* Number of registers contained in it, for easy rrn association*/
} B+Tree;


/* The Node struct that we will have in RAM */
typedef struct{
  int id;  /*id of this Node*/
  int leaf; /* 1 if leaf, 0 otherwise */
  int n; /* number of keys */
  int * keys; /* array of n keys (needs initialization) */
  int * pointers; /* array of n + 1 pointers to other Nodes */
  int prev; /* previous Node */
  int next; /* next Node */
} Node;


/* Return a pointer to a new allocated B+Tree. */
B+Tree * getNewB+Tree();


/* Return a pointer to a new allocated Node. */
/* Also allocates the arrays inside the Node. */
Node * getNewNode(int n);


/* Return 0 if wrote ok, 1 otherwize */
int writeNode(Node* no, int filename);


/* Reads the info from 'filename' to no */
int readNode(Node* no, int filename);


/* Returns 1 if Node 'no' has a underflow or overflow. */
int hasOverflowUnderflow(Node* no);


