#ifndef __MEMINDEX_H
#define __MEMINDEX_H

typedef struct {
  int id;
  char name[TITLE_LENGTH+1];
} MemoryIndexRecord;

typedef struct {
  const char *fp_name;
  int regnum;
  int maxregs;
  MemoryIndexRecord *reclist;
} MemoryIndex;

int           memory_index_find_id (MemoryIndex *index, const char *name);
void          memory_index_free (MemoryIndex *index);
void          memory_index_insert (MemoryIndex *index, const char *name);
MemoryIndex*  memory_index_new (size_t nelem);
void          memory_index_remove (MemoryIndex *index, int id);

#endif
