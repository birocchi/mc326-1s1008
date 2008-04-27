#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "memindex.h"

static void
flush_to_disk (MemoryIndex *index)
{
  int i;
  FILE *fp;

  assert (index != NULL);

  fp = fopen (index->fp_name, "w");
  assert (fp != NULL);

  for (i = 0; i < index->regnum; i++)
    {
      fprintf (fp, "%-200s%04d", index->reclist[i].name,
                                 index->reclist[i].id);
    }

  fclose (fp);
}

static void
inflate_list (MemoryIndex *index)
{
  MemoryIndexRecord *tmp;

  assert (index != NULL);

  tmp = realloc(index->reclist, (index->maxregs*2) *
                                sizeof (MemoryIndexRecord));
  assert(tmp != NULL);

  index->reclist = tmp;
  index->maxregs *= 2;
}

static int
bsearch_find_by_name (const void* a, const void* b)
{
  return strcasecmp ((char*)a, ((MemoryIndexRecord*)b)->name);
}

static int
qsort_compare_names (const void *a, const void *b)
{
  return strncmp (((MemoryIndexRecord*)a)->name,
                  ((MemoryIndexRecord*)b)->name, TITLE_LENGTH);
}

int
memory_index_find_id (MemoryIndex *index, const char *name)
{
  MemoryIndexRecord *match;

  assert (index != NULL);

  match = bsearch (name, index->reclist, index->regnum,
                   sizeof (MemoryIndexRecord), bsearch_find_by_name);
  if (match)
    return match->id;
  else
    return -1;
}

void
memory_index_free (MemoryIndex *index)
{
  if (index)
    {
      flush_to_disk (index);
      free (index->reclist);
      free (index);
    }
}

void
memory_index_insert (MemoryIndex *index, const char *name)
{
  assert (index != NULL);

  if (index->regnum == index->maxregs)
    inflate_list (index);

  index->reclist[index->regnum].id = index->regnum;
  index->regnum++;

  strncpy (index->reclist[index->regnum].name, name, TITLE_LENGTH+1);

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), qsort_compare_names);
}

MemoryIndex *
memory_index_new (size_t nelem)
{
  MemoryIndex *index = MEM_ALLOC (MemoryIndex);

  index->regnum = nelem;
  index->maxregs = (nelem == 0 ? 40 : 2*nelem);
  index->reclist = MEM_ALLOC_N (MemoryIndexRecord, index->maxregs);

  return index;
}

void
memory_index_remove (MemoryIndex *index, int id)
{
  int i = 0, j;

  while ((i < index->regnum) && (index->reclist[i].id != id))
    i++;

  if (i == index->regnum) /* Match not found, leave function */
    return;

  for (j = i; j < index->regnum - 1; j++)
    index->reclist[j] = index->reclist[j+1];

  index->regnum--;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), qsort_compare_names);
}
