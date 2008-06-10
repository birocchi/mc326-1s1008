#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "avail.h"
#include "base.h"
#include "file.h"
#include "hash.h"
#include "io.h"
#include "mem.h"
#include "memindex.h"
#include "secindex.h"

void
secondary_index_foreach (SecondaryIndex * index, MemoryIndexRecord * record,
                         void (*callback) (char *, int, va_list), ...)
{
  char tmpname[TITLE_LENGTH + 1];
  int node, nextnode;
  va_list ap;

  va_start (ap, callback);

  /* Iterate over the entry file list */
  node = record->rrn;
  while (node != -1)
    {
      /* Read the entry */
      fseek (index->fp_list, node * MEM_REG_SIZE, SEEK_SET);
      fgets (tmpname, TITLE_LENGTH + 1, index->fp_list);
      fread (&nextnode, sizeof (int), 1, index->fp_list);

      stripWhiteSpace (tmpname);

      /* Call the function with name, RRN and information */
      callback (tmpname, node, ap);

      node = nextnode;
    }

  va_end (ap);
}

void
secondary_index_free (SecondaryIndex * index)
{
  if (index)
    {
      avail_list_free (index->avlist);
      memory_index_free (index->record_list);
      fclose (index->fp_list);
      free (index);
    }
}

void
secondary_index_insert (SecondaryIndex * si_index, char *si_value,
                        const char *pk_value)
{
  int nextnode, newrrn, writepos;
  MemoryIndexRecord *rec;

  /* If there's no position in the avail list,
   * insert in the end of the file */
  if (avail_list_is_empty (si_index->avlist))
    {
      fseek (si_index->fp_list, 0, SEEK_END);
      newrrn = file_get_size (si_index->fp_list) / MEM_REG_SIZE;
    }
  else
    {
      /* Otherwise, seek to an available position */
      writepos = avail_list_pop (si_index->avlist, si_index->fp_list) *
        MEM_REG_SIZE;
      fseek (si_index->fp_list, writepos, SEEK_SET);
      newrrn = writepos / MEM_REG_SIZE;
    }

  /* Check whether or not to append to an existing record. */
  rec = memory_index_find (si_index->record_list, si_value);
  if (!rec)
    {
      memory_index_insert (si_index->record_list, si_value, newrrn);
      nextnode = -1;            /* No next position */
    }
  else
    {
      nextnode = rec->rrn;      /* This node is the new head of the list. */
      rec->rrn = newrrn;
    }

  /* Write the data into the file */
  fprintf (si_index->fp_list, "%-200s", pk_value);
  fwrite (&nextnode, sizeof (int), 1, si_index->fp_list);
  fflush (si_index->fp_list);
}

SecondaryIndex *
secondary_index_new (const char *indexname, const char *listname, const char
  *avname, unsigned int (*hash_func)(char*), int overwrite_index)
{
  SecondaryIndex *s = MEM_ALLOC (SecondaryIndex);

  s->record_list = memory_index_new (indexname, hash_func);

  s->avlist = avail_list_new (avname);
  avail_list_load (s->avlist);

  s->fp_list = fopen (listname, (overwrite_index ? "w+" : "r+"));
  assert (s->fp_list);

  return s;
}

void
secondary_index_remove (SecondaryIndex * index, char *sec_value,
                        const char *pk_value)
{
  MemoryIndexRecord *rec;
  char tmpname[TITLE_LENGTH + 1];
  int prevnode = -1, curnode, nextnode;

  assert (index);

  /* Check if this is a valid record in the index */
  rec = memory_index_find (index->record_list, sec_value);
  if (rec)
    {
      curnode = rec->rrn;

      /* Iterate over the entry list */
      while (curnode != -1)
        {
          /* Read the current entry */
          fseek (index->fp_list, curnode * MEM_REG_SIZE, SEEK_SET);
          fgets (tmpname, TITLE_LENGTH + 1, index->fp_list);
          fread (&nextnode, sizeof (int), 1, index->fp_list);

          stripWhiteSpace (tmpname);

          /* Check if this is really the match we are looking for */
          if (!strcasecmp (tmpname, pk_value))
            {
              /* Check whether this is the head of the list */
              if (prevnode == -1)
                {
                  /* If this is the head and there's no next item,
                   * we can delete the entry from the memory list */
                  if (nextnode == -1)
                    memory_index_remove (index->record_list, rec->name);
                  else          /* Otherwise set the new head */
                    rec->rrn = nextnode;
                }
              else
                {
                  /* Not the head, just make the previous node point to
                   * the next one and skip the current item */
                  fseek (index->fp_list,
                         (prevnode * MEM_REG_SIZE) + TITLE_LENGTH, SEEK_SET);
                  fwrite (&nextnode, sizeof (int), 1, index->fp_list);
                }

              /* Insert the position into the avail list */
              avail_list_push (index->avlist, index->fp_list, curnode *
                               MEM_REG_SIZE);

              break;
            }

          prevnode = curnode;
          curnode = nextnode;
        }
    }
}
