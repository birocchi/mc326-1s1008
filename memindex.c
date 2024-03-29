#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "base.h"
#include "file.h"
#include "filelist.h"
#include "hash.h"
#include "io.h"
#include "mem.h"
#include "memindex.h"

/**
 * @brief Binary search comparison function.
 *
 * @param a A string.
 * @param b A \a MemoryIndexRecord.
 *
 * @retval -1 a < b.
 * @retval 0  a == b.
 * @retval 1  a > b.
 */
static int bsearch_compare (const void *a, const void *b);

/**
 * @brief Changes the loaded hash file according to the given hash number.
 *
 * @param index   The memory index being used.
 * @param hashnum The hash returned by the hash function.
 */
static void change_hash_file (MemoryIndex * index, unsigned int hashnum);

/**
 * @brief Serializes a memory index to disk.
 *
 * @param index The index to be serialized.
 */
static void flush_to_disk (MemoryIndex * index);

/**
 * @brief Allocate more memory for a memory index.
 *
 * @param index The memory index.
 * @param size  The new size.
 *
 * If \a size is smaller than the current size,
 * this function does nothing.
 */
static void inflate_list (MemoryIndex * index, size_t size);

/**
 * @brief Load data from disk.
 *
 * @param index    The memory index.
 * @param filename Name of the serialized index file.
 *
 * This function restores a previously saved memory index
 * into the current index.
 */
static void load_file (MemoryIndex * index, const char *filename);

/**
 * @brief Case-insensitive \a MemoryIndexRecord comparison.
 *
 * @param a Pointer to a \a MemoryIndexRecord.
 * @param b Pointer to another \a MemoryIndexRecord.
 *
 * @retval -1 a < b.
 * @retval 0  a == b.
 * @retval 1  a > b.
 */
static int qsort_compare (const void *a, const void *b);

static int
bsearch_compare (const void *a, const void *b)
{
  return strcasecmp ((char *) a, ((MemoryIndexRecord *) b)->name);
}

static void
change_hash_file (MemoryIndex * index, unsigned int hashnum)
{
  char *filename;

  if (index->loaded_file != hashnum)
    {
      /* Flush changes to the previous file before changing */
      if (index->loaded_file != -1)
        {
          flush_to_disk (index);

          free (index->reclist);
          index->reclist = NULL;
          index->regnum = index->maxregs = 0;
        }

      filename = hash_get_filename (index->fp_name, hashnum, INDEX_HASH_NUM);
      load_file (index, filename);
      free (filename);

      index->loaded_file = hashnum;
    }
}

static void
flush_to_disk (MemoryIndex * index)
{
  char *filename;
  int i;
  FILE *fp;

  assert (index);

  /* Gets the filename with the right extension (filename.hXXXX) */
  filename =
    hash_get_filename (index->fp_name, index->loaded_file, INDEX_HASH_NUM);

  fp = fopen (filename, "w");
  assert (fp);

  free (filename);

  for (i = 0; i < index->regnum; i++)
    {
      fprintf (fp, "%-200s", index->reclist[i].name);
      fwrite (&(index->reclist[i].rrn), sizeof (int), 1, fp);
    }

  fclose (fp);
}

static void
inflate_list (MemoryIndex * index, size_t size)
{
  MemoryIndexRecord *tmp;

  assert (index);

  /* If the list is empty, allocate 20 blocks */
  if (size == 0)
    size = 20;

  if (size > index->maxregs)
    {
      tmp = realloc (index->reclist, size * sizeof (MemoryIndexRecord));
      assert (tmp);

      index->reclist = tmp;
      index->maxregs = size;
    }
}

static void
load_file (MemoryIndex * index, const char *filename)
{
  FILE *fp = NULL;
  int i, rrn;
  size_t regnum;

  assert (index);

  /* If the file doesn't exist, create it and leave */
  if (!file_exists (filename))
    {
      fp = fopen (filename, "w");
      assert (fp);
      fclose (fp);
      return;
    }

  fp = fopen (filename, "r");
  assert (fp);

  /* Allocate all the necessary memory at once. */
  regnum = file_get_size (fp) / MEM_REG_SIZE;
  inflate_list (index, regnum);

  /* Add each record back to the index. */
  for (i = 0; i < regnum; i++)
    {
      fgets (index->reclist[i].name, TITLE_LENGTH + 1, fp);
      fread (&rrn, sizeof (int), 1, fp);

      stripWhiteSpace (index->reclist[i].name);

      index->reclist[i].rrn = rrn;
      index->regnum++;
    }

  fclose (fp);
}

static int
qsort_compare (const void *a, const void *b)
{
  return strcasecmp (((MemoryIndexRecord *) a)->name,
                     ((MemoryIndexRecord *) b)->name);
}

MemoryIndexRecord *
memory_index_find (MemoryIndex * index, char *name)
{
  assert (index);

  change_hash_file (index, index->hash_function (name));

  return bsearch (name, index->reclist, index->regnum,
                  sizeof (MemoryIndexRecord), bsearch_compare);
}

void
memory_index_free (MemoryIndex * index)
{
  if (index)
    {
      if (index->loaded_file != -1)
        flush_to_disk (index);
      free (index->fp_name);
      free (index->reclist);
      free (index);
    }
}

void
memory_index_insert (MemoryIndex * index, char *name, int rrn)
{
  assert (index && name);

  change_hash_file (index, index->hash_function (name));

  /* Allocate more memory if necessary */
  if (index->regnum == index->maxregs)
    inflate_list (index, index->maxregs * 2);

  /* Insert the new record's information */
  strncpy (index->reclist[index->regnum].name, name, TITLE_LENGTH + 1);
  index->reclist[index->regnum].rrn = rrn;

  index->regnum++;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), qsort_compare);
}

MemoryIndex *
memory_index_new (const char *fp_name)
{
  MemoryIndex *index = MEM_ALLOC (MemoryIndex);

  index->regnum = 0;
  index->maxregs = 0;
  index->reclist = NULL;

  index->fp_name = str_dup (fp_name);

  index->loaded_file = -1;
  index->hash_function = fnv1_hash;

  return index;
}

void
memory_index_remove (MemoryIndex * index, char *name)
{
  int i = 0, j;

  assert (index && name);

  change_hash_file (index, index->hash_function (name));

  while ((i < index->regnum)
         && (strcasecmp (index->reclist[i].name, name) != 0))
    i++;

  if (i == index->regnum)       /* Match not found, leave function */
    return;

  /* Copy each entry back one position */
  for (j = i; j < index->regnum - 1; j++)
    index->reclist[j] = index->reclist[j + 1];

  index->regnum--;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), qsort_compare);
}
