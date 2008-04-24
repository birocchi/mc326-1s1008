#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "mem.h"
#include "secindex.h"
  
static void
secondary_index_inflate_list (SecondaryIndex* index)
{
  SecondaryIndexRecord* tmp;

  tmp = realloc (index->record_list, (index->max_records*2) * sizeof (SecondaryIndexRecord));
  assert (tmp != NULL);

  index->record_list = tmp;
  index->max_records *= 2;
}

static SecondaryIndexRecord *
secondary_index_insert_record (SecondaryIndex *index, const char *si_value)
{
  int curpos;
  SecondaryIndexRecord* s;

  if (index->total_records == index->max_records)
    secondary_index_inflate_list(index);

  curpos = index->total_records;
  s = &(index->record_list[curpos]);

  strncpy(s->title, si_value, TITLE_LENGTH+1);
  s->tail = -1;

  index->total_records++;

  return s;
}

void
secondary_index_insert (SecondaryIndex *si_index, const char *si_value, const char *pk_value)
{
  SecondaryIndexRecord *s;

  s = secondary_index_find_record(si_index, si_value);
  if (!s)
    s = secondary_index_insert_record(si_index->record_list, si_value);

  fprintf(si_index->fp_list, "%-200s", pk_value);
  fprintf(si_index->fp_list, "%04d", s->tail);

  s->tail = si_index->total_records;
  si_index->total_records++;
}

SecondaryIndex *
secondary_index_new (const char *indexname, const char *listname, const char *avname)
{
  SecondaryIndex *s = MEM_ALLOC (SecondaryIndex);

  s->avlist = avail_list_new (avname);

  s->fp_list = fopen (listname, "r+");
  assert (s->fp_list != NULL);

  s->total_records = 0;
  s->max_records = 40;
  s->record_list = MEM_ALLOC_N (SecondaryIndexRecord, s->max_records);
}
