#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "memindex.h"
#include "secindex.h"

void
secondary_index_free (SecondaryIndex *index)
{
  if (index)
    {
      avail_list_free (s->avlist);
      memory_index_free (s->record_list);
      fclose (index->fp_list);
      free (index->fp_index_name);
      free (index->record_list);
      free (index);
    }
}

void
secondary_index_insert (SecondaryIndex *si_index, const char *si_value, const char *pk_value)
{
  int listpos;
  int writepos;
  SecondaryIndexRecord *rec;

  rec = memory_index_find (si_index, si_value);
  if (rec)
    newrrn = rec->regnum + 1;
  else
    {
      rec = memory_index_insert (si_index, si_value);
      newrrn = rec->regnum;
    }

  if (avail_list_is_empty (si_index->avlist))
    fseek (si_index->fp_list, 0, SEEK_END);
  else
    {
      writepos = avail_list_pop (si_index->avlist, (PK_REG_SIZE), si_index->fp_list);
      fseek (base->fp, writepos, SEEK_SET);
    }

  fprintf (si_index->fp_list, "%-200s%04d", pk_value, rec->rrn);
  fflush (si_index->fp_list);

  rec->rrn = newrrn;
}

SecondaryIndex *
secondary_index_new (const char *indexname, const char *listname, const char *avname)
{
  SecondaryIndex *s = MEM_ALLOC (SecondaryIndex);

  s->avlist = avail_list_new (avname);
  s->record_list = memory_index_new (indexname, 0);

  s->fp_list = fopen (listname, "r+");
  assert (s->fp_list != NULL);

  return s;
}
