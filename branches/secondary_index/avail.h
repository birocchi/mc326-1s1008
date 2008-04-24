#ifndef __AVAIL_H
#define __AVAIL_H

#include "base.h"

typedef struct {
  FILE* fp;
  unsigned char *filename;
  int tail;
} AvailList;

int removedField(FILE * base, int rrn, int * avail);

void        avail_list_add (AvailList *avlist, int pos);
void        avail_list_free (AvailList *avlist);
int         avail_list_get_tail (AvailList *avlist);
int         avail_list_is_empty (AvailList *avlist);
AvailList*  avail_list_new (const char *filename);

#endif
