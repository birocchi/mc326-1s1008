/* Must be defined to have access to fileno() */
#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "file.h"

int fileExists(const char* filename) {
  struct stat buf;

  /* Check if stat return OK and the file is a regular file */
  if ((stat(filename, &buf) != 0) || (!S_ISREG(buf.st_mode)))
    return 0;

  return 1;
}

long getFileSize(FILE* f) {
  int fd;
  struct stat buf;

  fd = fileno(f);

  if (fstat(fd, &buf) != 0)
    return -1;

  return buf.st_size;
}

long getFileSizeFromName(const char* filename) {
  struct stat buf;

  /* Check if stat return OK and the file is a regular file */
  if ((stat(filename, &buf) != 0) || (!S_ISREG(buf.st_mode)))
    return -1;

  return buf.st_size;
}

int
isValidFile (const char *filename)
{
  return (fileExists (filename) && (getFileSizeFromName (filename) > 0));
}
