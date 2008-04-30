#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "avail.h"

static void
avail_list_write (AvailList * avlist)
{
  FILE *fp;

  /* Checking if it's ok. */
  assert (avlist != NULL);

  fp = fopen (avlist->filename, "w");
  assert (fp != NULL);
  /* If it opened all-right, then right the tail to it. */
  fprintf (fp, "%d", avlist->tail);
  fclose (fp);
}

void
avail_list_free (AvailList * avlist)
{
  if (avlist)
    /* If it was allocated, free it. */
    {
      avail_list_write (avlist);
      free (avlist->filename);
      free (avlist);
    }
}

int
avail_list_get_tail (AvailList * avlist)
{
  assert (avlist != NULL);
  
  /* If it exists, return the last position of it. */

  return avlist->tail;
}

int
avail_list_is_empty (AvailList * avlist)
{
  assert (avlist != NULL);

  /* Returns 0 or 1 whether the avail list
   is empty or not. */

  return avlist->tail == -1;
}

void
avail_list_load (AvailList * avlist)
{
  FILE *fp;

  assert (avlist);

  /* Check for existence.  */
  if (!fileExists (avlist->filename))
    return;
  
  /* Open it, check it, read it, close it. */
  fp = fopen (avlist->filename, "r");
  assert (fp);
  fscanf (fp, "%d", &(avlist->tail));
  fclose (fp);
}

AvailList *
avail_list_new (const char *filename, size_t page_size)
{
  AvailList *avlist;

  /* Fills the parts of the AvailList struct,
     then return a new copy of it. */

  avlist = MEM_ALLOC (AvailList);
  avlist->filename = str_dup (filename);
  avlist->page_size = page_size;
  avlist->tail = -1;

  /* Must free it later on. */
  return avlist;
}

int
avail_list_pop (AvailList * avlist, FILE * fp)
{
  int prevpos;

  /* Checks for consistency. */
  assert (avlist != NULL && fp != NULL);

  /* If it's not empty... */
  if (avlist->tail != -1)
    {
      prevpos = avlist->tail * avlist->page_size;

      /* Go to the position */
      fseek (fp, prevpos, SEEK_SET);
      /* Read it. */
      fscanf (fp, "%d", &(avlist->tail));

      /* Return the position. */
      return prevpos;
    }
  else
    /* If it didn't find anything. */
    return -1;
}

void
avail_list_push (AvailList * avlist, int pos)
{
  /* New tail is the new available position. */
  avlist->tail = pos;
}
