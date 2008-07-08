/* Must be defined to have access to fileno() */
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "file.h"
#include "hash.h"

int
file_exists (const char *filename)
{
  struct stat buf;

  /* Check if stat return OK and the file is a regular file */
  if ((stat (filename, &buf) != 0) || (!S_ISREG (buf.st_mode)))
    return 0;

  return 1;
}

long
file_get_size (FILE * f)
{
  int fd;
  struct stat buf;

  /* Get the file descriptor and stat it to get information */
  fd = fileno (f);
  if (fstat (fd, &buf) != 0)
    return -1;

  return buf.st_size;
}

long
file_get_size_from_name (const char *filename)
{
  struct stat buf;

  /* Check if stat returns OK and the file is a regular file */
  if ((stat (filename, &buf) != 0) || (!S_ISREG (buf.st_mode)))
    return -1;

  return buf.st_size;
}

int
file_is_valid (const char *filename)
{
  return (file_exists (filename) && (file_get_size_from_name (filename) > 0));
}
