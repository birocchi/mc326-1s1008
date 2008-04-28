#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "file.h"
#include "io.h"
#include "memindex.h"
#include "pk.h"

MemoryIndex *
pk_load_from_base (const char *base_name, const char *pkname)
{
  MemoryIndex *index;
  FILE *base;
  int i;

  base = fopen (base_name, "r");
  assert (base != NULL);

  index = memory_index_new (pkname, getFileSize (base) / BASE_REG_SIZE);

  for (i = 0; i < index->regnum; i++) {
    fgets(index->reclist[i].name, TITLE_LENGTH+1, base);
    index->reclist[i].rrn = i;

    /* Strip trailing whitespaces from the name */
    stripWhiteSpace(index->reclist[i].name);

    /* Skip the rest of the entry and go to the next record */
    fseek(base, BASE_REG_SIZE - TITLE_LENGTH, SEEK_CUR);
  }

  qsort (index->reclist, index->regnum, sizeof (MemoryIndexRecord),
         memory_index_compare_by_name);

  fclose(base);

  return index;
}
