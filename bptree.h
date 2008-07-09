#ifndef __BPTREE_H
#define __BPTREE_H

#define BP_NEXTID_FILE "nextid.bpx" /* The file with the next available ID */
#define BP_ROOTID_FILE "0"      /* The root node name (constant) */
#define MAX_FILENAME_LENGTH 255 /* Usual max size for a file name in a filesystem */

#define BPTREE_ORDER 6 /* The order of the tree */

typedef enum
{
  BP_TYPE_NODE,
  BP_TYPE_LEAF
} BPNodeType;

typedef struct
{
  unsigned int id;
  unsigned int maxsize;
  unsigned int usedsize;
  unsigned int left;
  unsigned int right;
  BPNodeType type;

  int *keys;
  int *values;
} BPNode;

typedef struct
{
  BPNode *root;
} BPTree;

void bptree_free (BPTree *tree);
unsigned int bptree_get_next_id (void);
void bptree_insert (BPTree *tree, int key, int value);
BPTree *bptree_new (void);
void bptree_update_root (BPTree *tree, BPNode *newroot);
void bpnode_free (BPNode *node);
unsigned int bpnode_get_id (BPNode *node);
unsigned int bpnode_get_left_node (BPNode *node);
unsigned int bpnode_get_maxsize (BPNode *node);
unsigned int bpnode_get_right_node (BPNode *node);
unsigned int bpnode_get_usedsize (BPNode *node);
BPNodeType bpnode_get_type (BPNode *node);
int bpnode_insert (BPNode *node, int key, int value);
int bpnode_is_full (BPNode *node);
int bpnode_is_leaf (BPNode *node);
void bpnode_marshal (BPNode *node);
BPNode *bpnode_new (BPNodeType type, int get_new_id);
int bpnode_rotate_left (BPNode *node, unsigned int *id);
int bpnode_rotate_right (BPNode *node, unsigned int *id);
int bpnode_search (BPNode *node, int key, int *not_found);
void bpnode_split (BPNode *curnode, BPNode *newnode, unsigned int *midpos);
BPNode *bpnode_unmarshal (unsigned int id);

#endif
