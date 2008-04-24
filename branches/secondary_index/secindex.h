#ifndef __SECINDEX_H
#define __SECINDEX_H

#include <stdio.h>
#include <stdlib.h>
#include "avail.h"
#include "base.h"

typedef struct {
  int tail;
  char title[TITLE_LENGTH+1];
} SecondaryIndexRecord;

typedef struct {
  AvailList *avlist;
  FILE* fp_index;
  FILE* fp_list;
  size_t max_records;
  size_t total_records;
  SecondaryIndexRecord* record_list;
} SecondaryIndex;

#endif
