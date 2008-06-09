#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "filelist.h"
#include "hash.h"
#include "io.h"

unsigned int
hash_function (char *key)
{
  unsigned int retval = 0;

  while (*key)
    {
      retval *= 0x01000193;
      retval ^= tolower (*key++);
    }

  return retval % INDEX_HASH_NUM;
}

char *
hash_get_filename (const char *prefix, unsigned int hashnum, unsigned int maxhash)
{
  char filename[255];           /* 255 bytes is usually the maximum length of a filename */
  int count;

  assert (hashnum < maxhash);

  count = sprintf (filename, "%s.h%04u", prefix, hashnum);
  assert (count <= 255);

  return str_dup (filename);
}
