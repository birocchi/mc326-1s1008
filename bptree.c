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
  unsigned int maxsize;
  unsigned int usedsize;
  unsigned int left;
  unsigned int right;

  int *keys;
  int *values;
} BPNode;

static unsigned int
get_position_for (BPTree *tree, int key)
{
  unsigned int i;

  for (i = 0; (i < tree->keycount) && (tree->keys[i] < key); i++);

  return i;
}

static char *
make_node_name (unsigned int id)
{
  char filename[MAX_FILENAME_LENGTH];
  int count;

  count = sprintf (filename, "%u.bpx", id);
  bpassert (count <= MAX_FILENAME_LENGTH);

  return str_dup (filename);
}

static void
shift_right (BPNode *node, unsigned int pos)
{
  if (!bpnode_is_leaf (node))
    node->values[node->usedsize + 1] = node->values[node->usedsize];

  for (i = node->usedsize; i > pos; i--)
    {
      node->keys[i] = node->keys[i-1];
      node->values[i] = node->values[i-1];
    }

  node->usedsize++;
}

void
bptree_insert (BPTree *tree, int key, int value)
{
  int has_overflowed;
  unsigned int midpos = 0;
  BPNode *newnode, *newroot;
  BPNode *root = tree->root;

  has_overflowed = bpnode_insert (root, key, value);
  if (has_overflowed) /* root overflow */
    {
      newnode = bpnode_new (bpnode_get_type (root));
      newroot = bpnode_new (BP_TYPE_NODE);

      bpnode_split (root, newnode, &midpos);

      newroot->keys[0] = root->keys[midpos];
      newroot->values[0] = bpnode_get_id (root);
      newroot->values[1] = bpnode_get_id (newnode);
      newroot->usedsize = 2;

      /* Serialize and update */
      bpnode_marshal (newroot);
      bptree_update_root (tree, newroot);

      bpnode_free (root);
      bpnode_free (newnode);
    }
}

void
bptree_update_root (BPTree *tree, BPNode *newroot)
{
  bpassert (tree && newroot);

  tree->root = newroot;
}

void
bpnode_free (BPNode *node)
{
  bpassert (node);

  bpnode_marshal (node);

  free (node->keys);
  free (node->values);
  free (node);
}

unsigned int
bpnode_get_id (BPNode *node)
{
  bpassert (node);
  return node->id;
}

unsigned int
bpnode_get_left_node (BPNode *node)
{
  bpassert (node);
  return node->left;
}

unsigned int
bpnode_get_maxsize (BPNode *node)
{
  bpassert (node);
  return node->maxsize;
}

unsigned int
bpnode_get_right_node (BPNode *node)
{
  bpassert (node);
  return node->right;
}

unsigned int
bpnode_get_usedsize (BPNode *node)
{
  bpassert (node);
  return node->usedsize;
}

BPNodeType
bpnode_get_type (BPNode *node)
{
  bpassert (node);

  return node->type;
}

BPNode *
find_leaf_node (BPNode *node, int key)
{
  BPNode *ret = node, *tmpnode;
  unsigned int pos;

  bpassert (node);

  if (bpnode_is_leaf (node))
    return node;
  else
    {
      pos = get_position_for (node, key);
      tmpnode = bpnode_unmarshal (node->values[pos]);
      ret = find_leaf_node (tmpnode, key);
      bpnode_free (tmpnode);
    }

  return ret;
}

int
bpnode_insert (BPNode *node, int key, int value)
{
  BPNode *newnode, *childnode;
  int child_overflow = 0;
  unsigned int id = 0, pos, midpos = 0;

  if (bpnode_is_leaf (node))
    {
      pos = get_position_for (node, key);

      if ((pos > 0) && (node->keys[pos-1] == key))
        return 0;

      shift_right (node, pos);

      node->keys[pos] = key;
      node->values[pos] = value;

      if (bpnode_is_full (node))
        has_overflowed = 1;
    }
  else
    {
      pos = get_position_for (node, key);
      childnode = bpnode_unmarshal (node->values[pos]);

      child_overflow = bpnode_insert (childnode, key, value);
      if (child_overflow)
        {
          if (bpnode_rotate_left (childnode, &id)) /* First try to rotate left */
            node->keys[pos-1] = id;
          else if (bpnode_rotate_right (childnode, &id)) /* Then rotate right */
            node->keys[pos] = id;
          else /* Only then split the node */
            {
              newnode = bpnode_new (bpnode_get_type (childnode));
              bpnode_split (childnode, newnode);

              shift_right (node, pos, &midpos);
              node->keys[pos] = childnode->keys[midpos];
              node->values[pos + 1] = bpnode_get_id (newnode);

              bpnode_free (childnode);
              bpnode_free (newnode);
            }
        }
    }

  return bpnode_is_full (node);
}

int
bpnode_is_full (BPNode *node)
{
  return bpnode_get_usedsize (node) >= (bpnode_get_maxsize (node) - 1);
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
  unsigned int i, maxsize, usedsize;

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
    fwrite (&(node->keys[i]), sizeof (int), 1, fp);

  for (i = 0; i < usedsize; i++)
    fwrite (&(node->values[i]), sizeof (int), 1, fp);

  /* In case it's not a leaf node, write the last pointer */
  if (!bpnode_is_leaf (node))
    fwrite (&(node->values[usedsize]), sizeof (int), 1, fp);

  fclose (fp);
}

int
bpnode_rotate_left (BPNode *node, unsigned int *id)
{
  BPNode *leftnode;
  unsigned int i;

  bpassert (node);

  if ((node->left == 0) || (bpnode_is_full (left)) || (!bpnode_is_leaf (node)))
    return 0;

  leftnode = bpnode_unmarshal (node->left);
  leftnode->keys[leafnode->usedsize] = node->keys[0];
  leftnode->values[leafnode->usedsize] = node->values[0];
  leftnode->usedsize++;

  for (i = 0; i < node->usedsize; i++)
    {
      node->keys[i] = node->keys[i+1];
      node->values[i] = node->values[i+1];
    }

  *id = node->keys[0];

  return 1;
}

void
bpnode_split (BPNode *curnode, BPNode *newnode, unsigned int *midpos)
{
  unsigned int i, mid;

  bpassert (curnode && newnode);

  *midpos = bpnode_get_maxsize (curnode) / 2;
  mid = (bpnode_is_leaf (curnode) ? *midpos : *midpos + 1);

  for (i = 0, j = mid; j < curnode->maxsize; i++, j++)
    {
      newnode->keys[i] = curnode->keys[j];
      newnode->values[i] = curnode->values[j];
    }

  if (bpnode_is_leaf (curnode))
    curnode->usedsize = mid;
  else
    curnode->usedsize = mid - 1;

  newnode->usedsize = (bpnode_get_maxsize (curnode) - mid) + 1;

  newnode->left = bpnode_get_id (curnode);
  newnode->right = bpnode_get_right_node (curnode);
  curnode->right = bpnode_get_id (newnode);

  bpnode_marshal (curnode);
  bpnode_marshal (newnode);
}

BPNode *
bpnode_unmarshal (unsigned int id)
{
  BPNode *node;
  char *fp_name;
  FILE *fp;
  unsigned int i, maxsize, usedsize;

  node = bpnode_new (BP_TYPE_NODE);

  fp_name = make_node_name (id);
  bpassert (file_is_valid (fp_name));

  fp = fopen (fp_name, "r");
  bpassert (fp);

  node->id = fgetc (fp);
  fread (&(node->maxsize), sizeof (unsigned int), 1, fp);
  fread (&(node->usedsize), sizeof (unsigned int), 1, fp);
  fread (&(node->left), sizeof (unsigned int), 1, fp);
  fread (&(node->right), sizeof (unsigned int), 1, fp);

  /* Variable-length information */
  for (i = 0; i < node->usedsize; i++)
    fread (&(node->keys[i]), sizeof (int), 1, fp);

  for (i = 0; i < node->usedsize; i++)
    fread (&(node->values[i]), sizeof (int), 1, fp);

  /* In case it's not a leaf node, write the last pointer */
  if (!bpnode_is_leaf (node))
    fread (&(node->values[node->usedsize]), sizeof (int), 1, fp);

  free (fp_name);
  fclose (fp);

  return node;
}
