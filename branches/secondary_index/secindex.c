#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "io.h"
#include "mem.h"
#include "memindex.h"
#include "secindex.h"

void
secondary_index_foreach (SecondaryIndex *index, MemoryIndexRecord *record,
                         void (*callback)(const char*, int, va_list), ...)
{
  char tmpname[TITLE_LENGTH+1], tmprrn[RRN_LENGTH+1];
  int node, nextnode;
  va_list ap;

  va_start (ap, callback);

  node = record->rrn;
  while (node != -1)
    {
      fseek (index->fp_list, node * PK_REG_SIZE, SEEK_SET);
      fgets (tmpname, TITLE_LENGTH+1, index->fp_list);
      fgets (tmprrn, RRN_LENGTH+1, index->fp_list);

      stripWhiteSpace(tmpname);
      nextnode = atoi(tmprrn);

      callback (tmpname, nextnode, ap);

      node = nextnode;
    }

  va_end (ap);
}

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
      writepos = avail_list_pop (si_index->avlist, si_index->fp_list);
      fseek (base->fp, writepos, SEEK_SET);
    }

  fprintf (si_index->fp_list, "%-200s%04d", pk_value, rec->rrn);
  fflush (si_index->fp_list);

  rec->rrn = newrrn;
}

SecondaryIndex *
secondary_index_new (const char *indexname, const char *listname, const char *avname, int writeonly)
{
  SecondaryIndex *s = MEM_ALLOC (SecondaryIndex);

  s->avlist = avail_list_new (avname, PK_REG_SIZE);
  s->record_list = memory_index_new (indexname, 0);

  if (!writeonly)
    {
      memory_index_load_from_file (s->record_list, indexname);
      avail_list_load (s->avlist);
    }

  s->fp_list = fopen (listname, (!writeonly && fileExists(listname) ? "r+" : "w"));
  assert (s->fp_list);

  return s;
}

void
secondary_index_remove (SecondaryIndex *index, const char *sec_value, const char *pk_value)
{
  MemoryIndexRecord *rec;
  int prevnode = -1, curnode, nextnode;

  assert (index != NULL);

  rec = memory_index_find (index, sec_value);
  if (rec)
    {
      curnode = rec->rrn;

      while (curnode != -1)
        {
          fseek (index->fp_list, curnode * PK_REG_SIZE, SEEK_SET);
          fgets (tmpname, TITLE_LENGTH+1, index->fp_list);
          fgets (tmprrn, RRN_LENGTH+1, index->fp_list);

          stripWhiteSpace (tmpname);
          nextnode = atoi (tmprrn);

          if (!strcasecmp (tmpname, pk_value))
            {
              if (prevnode == -1)
                rec->rrn = nextnode;
              else  /* Not the head node, no need to update the index */
                {
                  fseek (index->fp_list, (prevnode * PK_REG_SIZE) + TITLE_LENGTH, SEEK_SET);
                  fprintf (index->fp_list, "%04d", nextnode);
                }

                avail_list_push (index->avlist, curnode);

              break;
            }

          prevnode = curnode;
          curnode  = nextnode;

          if (rec->rrn == -1)
           memory_index_remove (index->record_list, rec);
        }
    }
}
