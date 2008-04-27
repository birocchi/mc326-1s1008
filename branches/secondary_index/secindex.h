#ifndef __SECINDEX_H
#define __SECINDEX_H

#include <stdio.h>
#include <stdlib.h>
#include "avail.h"
#include "memindex.h"

typedef struct {
  AvailList       *avlist;
  const char      *fp_index_name;
  FILE            *fp_list;
  MemoryIndex     *record_list;
} SecondaryIndex;

void            secondary_index_free (SecondaryIndex *index);
void            secondary_index_insert (SecondaryIndex *si_index, const char *si_value, const char *pk_value);
SecondaryIndex* secondary_index_new (const char *indexname, const char *listname, const char *avname);
#endif
