#ifndef __ADAPTER_H
#define __ADAPTER_H

#include "base.h"
#include "memindex.h"
#include "secindex.h"
  typedef struct
{
  Base * base;
  MemoryIndex * pk_index;
  SecondaryIndex * author_index;
  SecondaryIndex * type_index;
  SecondaryIndex * year_index;
} Adapter;
void adapter_find (Adapter * db);
void adapter_free (Adapter * db);
void adapter_insert (Adapter * db);
void adapter_list (Adapter * db);
void adapter_load_files (Adapter * db);
Adapter * adapter_new (void);
void adapter_remove (Adapter * db);

#endif  /*  */
