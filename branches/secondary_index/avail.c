#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "pk.h"
#include "avail.h"

static void
avail_list_write (AvailList *avlist)
{
  FILE *fp;

  assert (avlist != NULL);

  fp = fopen (avlist->filename, "w");
  assert (fp != NULL);
  fprintf (fp, "%d", avlist->tail);
  fclose (fp);
}

void
avail_list_free (AvailList *avlist)
{
  if (avlist)
    {
      avail_list_write (avlist);
      free (avlist->filename);
      free (avlist);
    }
}

int
avail_list_get_tail (AvailList *avlist)
{
  assert (avlist != NULL);

  return avlist->tail;
}

int
avail_list_is_empty (AvailList *avlist)
{
  assert (avlist != NULL);

  return avlist->tail == -1;
}

void
avail_list_load (AvailList *avlist)
{
  FILE *fp;

  assert (avlist);

  if (!fileExists(avlist->filename))
    return;
  
  fp = fopen (avlist->filename, "r");
  assert (fp);
  fscanf (fp, "%04d", avlist->tail);
  fclose (fp);
}

AvailList *
avail_list_new (const char *filename, size_t page_size)
{
  AvailList *avlist;

  avlist = MEM_ALLOC (AvailList);
  avlist->filename = strdup (filename);
  avlist->page_size = page_size;
  avlist->tail = -1;
}

int
avail_list_pop (AvailList *avlist, FILE *fp)
{
  int prevpos;

  assert (avlist != NULL && fp != NULL);

  if (avlist->tail != -1)
    {
      prevpos = avlist->tail * avlist->page_size;

      fseek (fp, prevpos, SEEK_SET);
      fscanf (fp, "%d", &(avlist->tail));

      return prevpos;
    }
  else
    return -1;
}

void
avail_list_push (AvailList *avlist, int pos)
{
  avlist->tail = pos;
}
