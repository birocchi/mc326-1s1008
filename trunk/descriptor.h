#ifndef __DESCRIPTOR_H
#define __DESCRIPTOR_H

#include <stdlib.h>
#include "base.h"
#include "memindex.h"

#define DESC_HASH_NUM 9

typedef struct
{
  double similarity;
  char img[IMG_LENGTH + 1];
} SimilarityRecord;

typedef struct
{
  size_t maxregs;
  size_t regnum;
  SimilarityRecord *list;
} SimilarityList;

typedef struct
{
  char *fp_name;
  FILE *fp;
  unsigned int loaded_file;
} Descriptor;

void
descriptor_find (Descriptor * desc, char *imgname, MemoryIndex * pk,
                 FILE * base_fp, size_t maxresults);

void descriptor_free (Descriptor * desc);

void descriptor_insert (Descriptor * desc, const char *pkname, unsigned char d);

Descriptor *descriptor_new (const char *fp_name);

#endif
