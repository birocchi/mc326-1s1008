#ifndef __AVAIL_H
#define __AVAIL_H

#include <stdlib.h>

typedef struct
{
  char *filename;
  size_t page_size;
  int tail;
} AvailList;

void avail_list_free (AvailList * avlist);
int avail_list_get_tail (AvailList * avlist);
int avail_list_is_empty (AvailList * avlist);
void avail_list_load (AvailList * avlist);
AvailList *avail_list_new (const char *filename, size_t page_size);
int avail_list_pop (AvailList * avlist, FILE * fp);
void avail_list_push (AvailList * avlist, int pos);

#endif
