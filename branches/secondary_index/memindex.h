#ifndef __MEMINDEX_H
#define __MEMINDEX_H

typedef struct {
  int  rrn;
  char name[TITLE_LENGTH+1];
} MemoryIndexRecord;

typedef struct {
  const char *fp_name;
  int regnum;
  int maxregs;
  MemoryIndexRecord *reclist;
} MemoryIndex;

int                 memory_index_compare_by_name (const void *a, const void *b)
MemoryIndexRecord*  memory_index_find (MemoryIndex *index, const char *name);
void                memory_index_free (MemoryIndex *index);
MemoryIndexRecord*  memory_index_insert (MemoryIndex *index, const char *name);
int                 memory_index_is_empty (MemoryIndex *index);
void                memory_index_load_from_file (MemoryIndex *index, const char *filename);
MemoryIndex*        memory_index_new (const char *fp_name, size_t nelem);
void                memory_index_remove (MemoryIndex *index, MemoryIndexRecord *key);

#endif
