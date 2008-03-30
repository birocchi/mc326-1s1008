#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "file.h"

int fileExists(const char* filename) {
  struct stat buf;

  /* Get all sorts of info about the file. */
  if (stat(filename, &buf) != 0)
    return 0;

  /* Check if it's a regular file. */
  if (!S_ISREG(buf.st_mode))
    return 0;

  return 1;
}

int getFileSize(FILE * f){
  int file_size;
  int prev_pos;
  
  /* Don't try to get the size of a NULL pointer. */
  if(!f)
    return -1;

  /* Save the current pointer position. */
  prev_pos = ftell(f);

  /* Go to the end of the file. */
  fseek(f, 0, SEEK_END);
  /* So the pointer position is the file size. */
  file_size = ftell(f);
  
  /* Take the pointer back to it's original position. */
  fseek(f, prev_pos, SEEK_SET);

  return file_size;
}
