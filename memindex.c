#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "base.h"
#include "file.h"
#include "hash.h"
#include "io.h"
#include "mem.h"
#include "memindex.h"

static int bsearch_find_by_name (const void *a, const void *b);
static void change_hash_file (MemoryIndex * index, char *name);
static void flush_to_disk (MemoryIndex * index);
static void inflate_list (MemoryIndex * index, size_t size);
static void memory_index_load_from_file (MemoryIndex * index, const char
                                         *filename);

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
static int
bsearch_find_by_name (const void *a, const void *b)
{
  return strcasecmp ((char *) a, ((MemoryIndexRecord *) b)->name);
}

static void
change_hash_file (MemoryIndex * index, char *name)
{
  char *filename;
  unsigned int hashnum;

  hashnum = index->hash_function (name);

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

      filename = hash_get_filename (index->fp_name, hashnum);
      memory_index_load_from_file (index, filename);
      free (filename);

      index->loaded_file = hashnum;
    }
}

/**
 * @brief Serializes a memory index to disk.
 *
 * @param index The index to be serialized.
 */
static void
flush_to_disk (MemoryIndex * index)
{
  char *filename;
  int i;
  FILE *fp;

  assert (index);

  filename = hash_get_filename (index->fp_name, index->loaded_file);
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

/**
 * @brief Allocate more memory for a memory index.
 *
 * @param index The memory index.
 * @param size  The new size.
 *
 * If \a size is smaller than the current size,
 * this function does nothing.
 */
static void
inflate_list (MemoryIndex * index, size_t size)
{
  MemoryIndexRecord *tmp;

  assert (index);

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

/**
 * @brief Load data from disk.
 *
 * @param index    The memory index.
 * @param filename Name of the serialized index file.
 *
 * This function restores a previously saved memory index
 * into the current index.
 */
static void
memory_index_load_from_file (MemoryIndex * index, const char *filename)
{
  FILE *fp = NULL;
  int i, rrn;
  size_t regnum;

  assert (index);

  /* If the file doesn't exist, create it and leave */
  if (!fileExists (filename))
    {
      fp = fopen (filename, "w");
      assert (fp);
      fclose (fp);
      return;
    }

  fp = fopen (filename, "r");
  assert (fp);

  /* Allocate all the necessary memory at once. */
  regnum = getFileSize (fp) / MEM_REG_SIZE;
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

int
memory_index_compare_by_name (const void *a, const void *b)
{
  return strcasecmp (((MemoryIndexRecord *) a)->name,
                     ((MemoryIndexRecord *) b)->name);
}

MemoryIndexRecord *
memory_index_find (MemoryIndex * index, char *name)
{
  assert (index);

  change_hash_file (index, name);

  return bsearch (name, index->reclist, index->regnum,
                  sizeof (MemoryIndexRecord), bsearch_find_by_name);
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

  change_hash_file (index, name);

  /* Allocate more memory if necessary */
  if (index->regnum == index->maxregs)
    inflate_list (index, index->maxregs * 2);

  /* Insert the new record's information */
  strncpy (index->reclist[index->regnum].name, name, TITLE_LENGTH + 1);
  index->reclist[index->regnum].rrn = rrn;

  index->regnum++;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), memory_index_compare_by_name);
}

MemoryIndex *
memory_index_new (const char *fp_name)
{
  return memory_index_new_with_hash (fp_name, hash_function);
}

MemoryIndex *
memory_index_new_with_hash (const char *fp_name, unsigned int
                            (*hash_func) (char *))
{
  MemoryIndex *index = MEM_ALLOC (MemoryIndex);

  index->regnum = 0;
  index->maxregs = 0;
  index->reclist = NULL;

  index->fp_name = str_dup (fp_name);

  index->loaded_file = -1;
  index->hash_function = hash_func;

  return index;
}

void
memory_index_remove (MemoryIndex * index, int rrn)
{
  int i = 0, j;

  while ((i < index->regnum) && (index->reclist[i].rrn != rrn))
    i++;

  if (i == index->regnum)       /* Match not found, leave function */
    return;

  /* Copy each entry back one position */
  for (j = i; j < index->regnum - 1; j++)
    index->reclist[j] = index->reclist[j + 1];

  index->regnum--;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), memory_index_compare_by_name);
}
