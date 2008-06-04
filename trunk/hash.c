#include <assert.h>
#include <stdio.h>
#include "hash.h"
#include "io.h"

unsigned int
hash_function (char *key)
{
  unsigned int retval;

  while (*key)
    {
      retval *= 0x01000193;
      retval ^= *key++;
    }

  return retval % HASH_FILE_NUM;
}

char *
hash_get_filename (const char *prefix, unsigned int hashnum)
{
  char filename[255]; /* 255 bytes is usually the maximum length of a filename */
  int count;

  assert (hashnum < HASH_FILE_NUM);

  count = sprintf (filename, "%s.h%04u", prefix, hashnum);
  assert (count <= 255);

  return str_dup (filename);
}
