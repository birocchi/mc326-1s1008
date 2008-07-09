#include <stdlib.h>
#include <stdio.h>
#include "bptree.h"
#include "file.h"
#include "io.h"
#include "mem.h"

#ifdef DEBUG
#include <assert.h>
#define bpassert(x) assert(x)
#else
#define bpassert(x)
#endif

static unsigned int
get_position_for (BPNode *node, int key)
{
  unsigned int i;

  for (i = 0; (i < node->usedsize) && (node->keys[i] < key); i++);

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
  unsigned int i;

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
bptree_free (BPTree *tree)
{
  bpassert (tree);

  bpnode_free (tree->root);
  free (tree);
}

unsigned int
bptree_get_next_id (void)
{
  FILE *nextid;
  int created_now = 0;
  unsigned int id = 1;

  bpassert (tree);

  if (file_is_valid (BP_NEXTID_FILE))
    {
      nextid = fopen (BP_NEXTID_FILE, "r");
      bpassert (nextid);

      fread (&id, sizeof (unsigned int), 1, nextid);
      fclose (nextid);

      created_now = 1;
    }

  nextid = fopen (BP_NEXTID_FILE, "w");
  bpassert (nextid);

  if (!created_now)
    id++;

  fwrite (&id, sizeof (unsigned int), 1, nextid);

  return (created_now ? id : id - 1);
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

BPTree *
bptree_new (void)
{
  FILE *rootid_fp;
  unsigned int rootid;
  BPNode *root;
  BPTree *ret = MEM_ALLOC (BPTree);

  if (file_is_valid (BP_ROOTID_FILE))
    {
      rootid_fp = fopen (BP_ROOTID_FILE, "r");
      bpassert (rootid_fp);

      fread (&rootid, sizeof (unsigned int), 1, rootid_fp);

      ret->root = bpnode_unmarshal (rootid);

      fclose (rootid_fp);
    }
  else
    {
      root = bpnode_new (BP_TYPE_LEAF);
      bptree_update_root (ret, root);
    }

  return ret;
}

void
bptree_update_root (BPTree *tree, BPNode *newroot)
{
  FILE *rootid = fopen (BP_ROOTID_FILE, "w");
  unsigned int id;

  bpassert (tree && newroot && rootid);

  tree->root = newroot;
  id = bpnode_get_id (tree->root);
  fwrite (&id, sizeof (unsigned int), 1, rootid);

  fclose (rootid);
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
              bpnode_split (childnode, newnode, &midpos);

              shift_right (node, pos);
              node->keys[pos] = childnode->keys[midpos];
              node->values[pos + 1] = bpnode_get_id (newnode);

              bpnode_free (newnode);
            }
        }

      bpnode_free (childnode);
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
  BPNodeType t;
  FILE *fp;
  unsigned int i, usedsize;

  bpassert (node);

  fp = fopen (make_node_name (node->id), "w");
  bpassert (fp);

  usedsize = bpnode_get_usedsize (node);

  /* Write header (fixed-size information) */
  fputc (bpnode_get_type (node), fp);
  i = bpnode_get_maxsize (node);
  fwrite (&i, sizeof (unsigned int), 1, fp);
  fwrite (&usedsize, sizeof (unsigned int), 1, fp);
  i = bpnode_get_left_node (node);
  fwrite (&i, sizeof (unsigned int), 1, fp);
  i = bpnode_get_right_node (node);
  fwrite (&i, sizeof (unsigned int), 1, fp);
  t = bpnode_get_type (node);
  fwrite (&t, sizeof (BPNodeType), 1, fp);

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

BPNode *
bpnode_new (BPNodeType type)
{
  BPNode *ret = MEM_ALLOC (BPNode);

  ret->id = bptree_get_next_id ();
  ret->maxsize = BPTREE_ORDER;
  ret->usedsize = 0;
  ret->left = 0;
  ret->right = 0;

  ret->keys = MEM_ALLOC_N (int, BPTREE_ORDER + 1);
  ret->values = MEM_ALLOC_N (int, BPTREE_ORDER + 2);

  return ret;
}

int
bpnode_rotate_left (BPNode *node, unsigned int *id)
{
  BPNode *leftnode;
  unsigned int i;

  bpassert (node);

  if ((node->left == 0) || (!bpnode_is_leaf (node)))
    return 0;

  leftnode = bpnode_unmarshal (node->left);

  if (bpnode_is_full (leftnode))
    {
      bpnode_free (leftnode);
      return 0;
    }

  leftnode->keys[leftnode->usedsize] = node->keys[0];
  leftnode->values[leftnode->usedsize] = node->values[0];
  leftnode->usedsize++;

  for (i = 0; i < node->usedsize; i++)
    {
      node->keys[i] = node->keys[i+1];
      node->values[i] = node->values[i+1];
    }

  node->usedsize--;

  *id = node->keys[0];

  bpnode_free (leftnode);
  bpnode_marshal (node);

  return 1;
}

int
bpnode_rotate_right (BPNode *node, unsigned int *id)
{
  BPNode *rightnode;

  bpassert (node);

  if ((node->right == 0) || (!bpnode_is_leaf (node)))
    return 0;

  rightnode = bpnode_unmarshal (node->right);

  if (bpnode_is_full (rightnode))
    {
      bpnode_free (rightnode);
      return 0;
    }

  shift_right (rightnode, 0);
  rightnode->keys[0] = node->keys[node->usedsize];
  rightnode->values[0] = node->values[node->usedsize];

  node->usedsize--;

  *id = rightnode->keys[0];

  bpnode_free (rightnode);
  bpnode_marshal (node);

  return 1;
}

void
bpnode_split (BPNode *curnode, BPNode *newnode, unsigned int *midpos)
{
  unsigned int i, j, mid;

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
  unsigned int i;

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
  fread (&(node->type), sizeof (BPNodeType), 1, fp);

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
