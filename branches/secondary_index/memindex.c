#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "base.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "memindex.h"

static void
flush_to_disk (MemoryIndex * index)
{
  int i;
  FILE *fp;

  assert (index);

  fp = fopen (index->fp_name, "w");
  assert (fp);

  for (i = 0; i < index->regnum; i++)
    {
      fprintf (fp, "%-200s%04d", index->reclist[i].name,
               index->reclist[i].rrn);
    }

  fclose (fp);
}

static void
inflate_list (MemoryIndex * index, size_t size)
{
  MemoryIndexRecord *tmp;

  assert (index);
  assert (size > index->maxregs);

  if (size > index->maxregs)
    {
      tmp = realloc (index->reclist, size * sizeof (MemoryIndexRecord));
      assert (tmp);

      index->reclist = tmp;
      index->maxregs = size;
    }
}

static int
bsearch_find_by_name (const void *a, const void *b)
{
  return strcasecmp ((char *) a, ((MemoryIndexRecord *) b)->name);
}

int
memory_index_compare_by_name (const void *a, const void *b)
{
  return strcasecmp (((MemoryIndexRecord *) a)->name,
                     ((MemoryIndexRecord *) b)->name);
}

MemoryIndexRecord *
memory_index_find (MemoryIndex * index, const char *name)
{
  assert (index);

  return bsearch (name, index->reclist, index->regnum,
                  sizeof (MemoryIndexRecord), bsearch_find_by_name);
}

void
memory_index_free (MemoryIndex * index)
{
  if (index)
    {
      flush_to_disk (index);
      free (index->fp_name);
      free (index->reclist);
      free (index);
    }
}

void
memory_index_insert (MemoryIndex * index, const char *name, int rrn)
{
  assert (index && name);

  if (index->regnum == index->maxregs)
    inflate_list (index, index->maxregs * 2);

  strncpy (index->reclist[index->regnum].name, name, TITLE_LENGTH + 1);
  index->reclist[index->regnum].rrn = rrn;
  index->regnum++;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), memory_index_compare_by_name);
}

int
memory_index_is_empty (MemoryIndex * index)
{
  return (index ? index->regnum == 0 : 1);
}

void
memory_index_load_from_file (MemoryIndex * index, const char *filename)
{
  FILE *fp = NULL;
  char strrrn[RRN_LENGTH + 1];
  int i, rrn;
  size_t regnum;

  assert (index);

  if (!fileExists (filename))
    return;

  fp = fopen (filename, "r");
  assert (fp);

  regnum = getFileSize (fp) / MEM_REG_SIZE;
  inflate_list (index, regnum);

  for (i = 0; i < regnum; i++)
    {
      fgets (index->reclist[i].name, TITLE_LENGTH + 1, fp);
      fgets (strrrn, RRN_LENGTH + 1, fp);

      stripWhiteSpace (index->reclist[i].name);
      rrn = atoi (strrrn);

      index->reclist[i].rrn = rrn;
      index->regnum++;
    }

  fclose (fp);
}

MemoryIndex *
memory_index_new (const char *fp_name, size_t nelem)
{
  MemoryIndex *index = MEM_ALLOC (MemoryIndex);

  index->regnum = nelem;
  index->maxregs = (nelem == 0 ? 40 : 2 * nelem);
  index->reclist = MEM_ALLOC_N (MemoryIndexRecord, index->maxregs);
  index->fp_name = str_dup (fp_name);

  return index;
}

void
memory_index_remove (MemoryIndex * index, MemoryIndexRecord * key)
{
  int i = 0, j;

  while ((i < index->regnum) && (&index->reclist[i] != key))
    i++;

  if (i == index->regnum)       /* Match not found, leave function */
    return;

  for (j = i; j < index->regnum - 1; j++)
    index->reclist[j] = index->reclist[j + 1];

  index->regnum--;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), memory_index_compare_by_name);
}
