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
                                 index->reclist[i].rrn);
    }

  fclose (fp);
}

static void
inflate_list (MemoryIndex *index, size_t size)
{
  MemoryIndexRecord *tmp;

  assert (index != NULL);

  if (size > index->maxregs)
    {
      tmp = realloc(index->reclist, size * sizeof (MemoryIndexRecord));
      assert(tmp != NULL);

      index->reclist = tmp;
      index->maxregs = size;
    }
}

static int
bsearch_find_by_name (const void* a, const void* b)
{
  return strcasecmp ((char*)a, ((MemoryIndexRecord*)b)->name);
}

int
memory_index_compare_by_name (const void *a, const void *b)
{
  return strncmp (((MemoryIndexRecord*)a)->name,
                  ((MemoryIndexRecord*)b)->name, TITLE_LENGTH);
}

MemoryIndexRecord *
memory_index_find (MemoryIndex *index, const char *name)
{
  assert (index != NULL);

  return bsearch (name, index->reclist, index->regnum,
                  sizeof (MemoryIndexRecord), bsearch_find_by_name);
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

MemoryIndexRecord *
memory_index_insert (MemoryIndex *index, const char *name)
{
  assert (index != NULL);

  if (index->regnum == index->maxregs)
    inflate_list (index, index->maxregs * 2);

  index->reclist[index->regnum].rrn = index->regnum;
  index->regnum++;

  strncpy (index->reclist[index->regnum].name, name, TITLE_LENGTH+1);

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), memory_index_compare_by_names);

  /* Return the inserted entry's RRN */
  return index->reclist[index->regnum-1];
}

int
memory_index_is_empty (MemoryIndex *index)
{
  return (index ? index->regnum == 0 : 1);
}

void
memory_index_load_from_file (MemoryIndex *index, const char *filename)
{
  FILE *fp = NULL;
  char strrrn[RRN_LENGTH+1];
  int i, rrn;
  size_t regnum;

  assert (index != NULL);
  assert (fileExists (filename));

  fp = fopen (filename, "r");
  assert (fp != NULL);

  regnum = getFileSize (fp) / MEM_REG_SIZE;
  inflate_list (index, regnum);

  for (i = 0; i < regnum; i++ )
    {  
      fgets (index->reclist[i].name, TITLE_LENGTH+1, fp);

      /* Strip trailing whitespaces from the name */
      stripWhiteSpace(index->reclist[i].name);

      fgets(strid, RRN_LENGTH+1, fp);
      rrn = atoi(strrrn);
      index->reclist[i].rrn = rrn;
    }

  fclose (fp);
}

MemoryIndex *
memory_index_new (const char *fp_name, size_t nelem)
{
  MemoryIndex *index = MEM_ALLOC (MemoryIndex);

  index->regnum = nelem;
  index->maxregs = (nelem == 0 ? 40 : 2*nelem);
  index->reclist = MEM_ALLOC_N (MemoryIndexRecord, index->maxregs);
  index->fp_name = strdup (fp_name);

  return index;
}

void
memory_index_remove (MemoryIndex *index, MemoryIndexRecord *key)
{
  int i = 0, j;

  while ((i < index->regnum) && (index->reclist[i] != key))
    i++;

  if (i == index->regnum) /* Match not found, leave function */
    return;

  for (j = i; j < index->regnum - 1; j++)
    index->reclist[j] = index->reclist[j+1];

  index->regnum--;

  qsort (index->reclist, index->regnum,
         sizeof (MemoryIndexRecord), memory_index_compare_by_name);
}
