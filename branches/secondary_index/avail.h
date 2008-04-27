#ifndef __AVAIL_H
#define __AVAIL_H

#include "base.h"

typedef struct {
  const char  *filename;
  size_t      page_size;
  int         tail;
} AvailList;

int removedField(FILE * base, int rrn, int * avail);

void        avail_list_free (AvailList *avlist);
int         avail_list_get_tail (AvailList *avlist);
int         avail_list_is_empty (AvailList *avlist);
AvailList*  avail_list_new (const char *filename, size_t page_size);
void        avail_list_push (AvailList *avlist, int pos);

#endif
