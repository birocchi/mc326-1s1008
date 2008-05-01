#ifndef __MEMINDEX_H
#define __MEMINDEX_H

#include <stdlib.h>
#include "base.h"

/** Relative record number: 32-bit integer (4 bytes) */
#define RRN_LENGTH    4
#define MEM_REG_SIZE  (TITLE_LENGTH + RRN_LENGTH)

typedef struct
{
  int rrn;
  char name[TITLE_LENGTH + 1];
} MemoryIndexRecord;

typedef struct
{
  char *fp_name;
  size_t regnum;
  size_t maxregs;
  MemoryIndexRecord *reclist;
} MemoryIndex;

int memory_index_compare_by_name (const void *a, const void *b);
MemoryIndexRecord *memory_index_find (MemoryIndex * index, const char *name);
void memory_index_free (MemoryIndex * index);
void memory_index_insert (MemoryIndex * index, const char *name, int rrn);
int memory_index_is_empty (MemoryIndex * index);
void memory_index_load_from_file (MemoryIndex * index, const char *filename);
MemoryIndex *memory_index_new (const char *fp_name, size_t nelem);
void memory_index_remove (MemoryIndex * index, int rrn);

#endif
