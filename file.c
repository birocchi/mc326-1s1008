#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "file.h"

int fileExists(const char* filename) {
  struct stat buf;

  if (stat(filename, &buf) != 0)
    return 0;

  if (!S_ISREG(buf.st_mode))
    return 0;

  return 1;
}

int getFileSize(FILE * f){
  int file_size;
  int prev_pos;

  if(!f)
    return -1;

  prev_pos = ftell(f);

  fseek(f, 0, SEEK_END);
  file_size = ftell(f);

  fseek(f, prev_pos, SEEK_SET);

  return file_size;
}
