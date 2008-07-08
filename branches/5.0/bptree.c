#include <stdlib.h>
#include "mem.h"

#ifdef DEBUG
#include <assert.h>
#define bpassert(x) assert(x)
#else
#define bpassert(x)
#endif

enum
{
  BP_TYPE_NODE,
  BP_TYPE_LEAF
} BPNodeType;

typedef struct
{
  unsigned int id;
  unsigned int order;
  unsigned int usedsize;
  unsigned int left;
  unsigned int right;

  unsigned int *keys;
  unsigned int *values;
} BPNode;

static unsigned int
get_position_for (BPTree *tree, int key)
{
  unsigned int i;

  for (i = 0; (i < tree->keycount) && (tree->keys[i] < key); i++);

  return i;
}

static void
shift_right (BPTree *tree, unsigned int pos)
{
  unsigned int i = pos + 1;

  for (i = pos + 1; i < tree->keycount; i++)
    {
      tree->keys[i] = tree->keys[i-1];
      tree->values[i] = tree->values[i-1];
    }
}

void
bptree_insert (BPTree *tree, int key, int value)
{
  BPNode *newnode, *newroot;
  BPNode *root = tree->root;
  int has_overflowed;

  has_overflowed = bpnode_insert (root, key, value);
  if (has_overflowed) /* root overflow */
    {
      newnode = bpnode_new (bpnode_get_type (root));
      newroot = bpnode_new (BP_TYPE_NODE);

      bpnode_split (root, newnode);

      newroot->keys[0] = bpnode_get_largest_key (root);
      newroot->values[0] = bpnode_get_id (root);
      newroot->values[1] = bpnode_get_id (newnode);
      newroot->numkeys = 2;

      /* Serialize and update tree root */
      bpnode_marshal (newroot);
      bptree_update_root (tree, newroot);
    }
}

void
bptree_update_root (BPTree *tree, BPNode *newroot)
{
  bpassert (tree && newroot);

  tree->root = newroot;
}

BPNodeType
bpnode_get_type (BPNode *node)
{
  bpassert (node);

  return node->type;
}

void
bpnode_insert (BPTree *tree, int key, int value)
{
  unsigned int pos;

  if (bptree_is_leaf (tree))
    {
      if (!is_duplicate (tree, key))
        {
          pos = get_position_for (tree, key);
          shift_right (tree, pos);

          tree->keys[pos] = key;
          tree->values[pos] = value;
          tree->keycount++;
        }
    }
}

int
bpnode_is_leaf (BPNode *node)
{
  return bpnode_get_type (node) == BP_TYPE_LEAF;
}

void
bpnode_marshal (BPNode *node)
{
  FILE *fp;
  unsigned int maxsize, usedsize;

  bpassert (node);

  fp = fopen (bpnode_get_filename (node), "w");
  bpassert (fp);

  usedsize = bpnode_get_usedsize (node);

  /* Write header (fixed-size information) */
  fputc (bpnode_get_type (node), fp);
  fwrite (&(bpnode_get_maxsize (node)), sizeof (unsigned int), 1, fp);
  fwrite (&usedsize, sizeof (unsigned int), 1, fp);
  fwrite (&(bpnode_get_left_node (node)), sizeof (unsigned int), 1, fp);
  fwrite (&(bpnode_get_right_node (node)), sizeof (unsigned int), 1, fp);

  /* Variable-length information */
  for (i = 0; i < usedsize; i++)
    fwrite (&(node->keys[i]), sizeof (unsigned int), 1, fp);

  for (i = 0; i < usedsize; i++)
    fwrite (&(node->values[i]), sizeof (unsigned int), 1, fp);

  /* In case it's not a leaf node, write the last pointer */
  if (!bpnode_is_leaf (node))
    fwrite (&(node->values[usedsize]), sizeof (unsigned int), 1, fp);
}
