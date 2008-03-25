#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

int writeData(FILE *file, artwork_info *info)
{
  /* Return error if the file or struct pointers are NULL. */
  if (!file || !info) {
    return 1;
  }

  fprintf(file, "%-200s",   info->title);
  fprintf(file, "%-100s",   info->type);
  fprintf(file, "%-125s",   info->author);
  fprintf(file, "%04d",     info->year);
  fprintf(file, "%012d",    info->value);
  fprintf(file, "%s",       info->img);

  return 0;
}

int validateIdentifier(const char* name)
{
  char* endptr;
  int i;

  i = strtol(name, &endptr, 10);
  if ((endptr == name) || (endptr == '\0') || strlen(endptr) != 3 || ((strncmp(endptr, "jpg", 3)) &&
        strncmp(endptr, "gif", 3) && (strncmp(endptr, "png", 3))))
    return 1;
  else
    return 0;
}
