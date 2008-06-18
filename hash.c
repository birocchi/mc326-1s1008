#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "filelist.h"
#include "hash.h"
#include "io.h"

#define FNV1_OFFSET_BASIS 0x811c9dc5; /* FNV1 32-bit offset basis */
#define FNV1_PRIME        0x01000193; /* 32-bit FNV prime */

unsigned int
fnv1_hash (char *key)
{
  unsigned char *s = (unsigned char *)key;
  unsigned long hash = FNV1_OFFSET_BASIS;

  while (*s)
    {
      hash *= FNV1_PRIME;
      hash ^= tolower (*s++);
    }

  return hash % INDEX_HASH_NUM;
}

char *
hash_get_filename (const char *prefix, unsigned int hashnum,
                   unsigned int maxhash)
{
  char filename[255];           /* 255 bytes is usually the maximum length of a filename */
  int count;

  assert (hashnum < maxhash);

  count = sprintf (filename, "%s.h%04u", prefix, hashnum);
  assert (count <= 255);

  return str_dup (filename);
}
