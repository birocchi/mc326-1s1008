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

void
avail_list_add (AvailList *avlist, int pos)
{
  FILE *fp;

  assert (avlist != NULL);

  fp = fopen (avlist->filename, "w");
  assert (fp != NULL);
  fprintf (fp, "%d", avlist->tail);
  fclose (fp);

  avlist->tail = pos;
}

void
avail_list_free (AvailList *avlist)
{
  if (avlist) {
    free (avlist->filename);
    free (avlist);
  }
}

int
avail_list_get_tail (AvailList *avlist)
{
  assert(avlist != NULL);

  return avlist->tail;
}

int
avail_list_is_empty (AvailList *avlist)
{
  assert(avlist != NULL);

  return avlist->tail == -1;
}

AvailList *
avail_list_new (const char *filename)
{
  AvailList *avlist;

  avlist = MEM_ALLOC(AvailList);
  avlist->filename = strdup(filename);
  avlist->fp = NULL;
  avlist->tail = -1;
}

int
avail_list_pop (AvaiList *avlist, size_t page_size, FILE *fp)
{
  int prevpos;

  assert (avlist != NULL && fp != NULL);
  assert (page_size > 0);

  prevpos = avlist->tail * page_size;

  fseek  (fp, prevpos, SEEK_SET);
  fscanf (fp, "%d", &(avlist->tail));

  return prevpos;
}

int removedField(FILE * base, int rrn, int * avail){
  int opened;

  if(base){
    opened = 1;
    fclose(base);
    base = fopen(DBFILE, "r+");
  }
  else{
    opened = 0;
    base = fopen(DBFILE, "r+");
  }

  fseek(base, BASE_REG_SIZE * rrn, SEEK_SET);
  fprintf(base, "%09d ", *avail);
  *avail = rrn;
  
  fclose(base);

  if(opened){
    base = fopen(DBFILE, "r+");
  }

  return 0;

}
