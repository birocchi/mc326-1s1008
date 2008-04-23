#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "io.h"
#include "mem.h"

char* baseGetValidImagePath(const char* s) {
  char* file;

  file = MEM_ALLOC_N(char, IMG_LENGTH+2);

  strncpy(file, s, 6);
  file[6] = '.';
  strncpy(file+7, s+6, 3);
  file[10] = '\0';

  return file;
}

int baseIsValidIdentifier(const char* name) {
  char* endptr;
  int i;

  i = strtol(name, &endptr, 10);
  if ((endptr == name) || (endptr == '\0') || strlen(endptr) != 3 || ((strncmp(endptr, "jpg", 3)) &&
        strncmp(endptr, "gif", 3) && (strncmp(endptr, "png", 3))))
    return 1;
  else
    return 0;
}

int baseReadArtworkRecord(FILE *base, artwork_info *info)
{
  if (!base || !info) {
    return 1;
  }

  fgets(info->title, TITLE_LENGTH+1, base);
  stripWhiteSpace(info->title);
  fgets(info->type, TYPE_LENGTH+1, base);
  stripWhiteSpace(info->type);
  fgets(info->author, AUTHOR_LENGTH+1, base);
  stripWhiteSpace(info->author);
  fgets(info->year, YEAR_LENGTH+1, base);
  stripWhiteSpace(info->year);
  fgets(info->value, VALUE_LENGTH+1, base);
  stripWhiteSpace(info->value);
  fgets(info->img, IMG_LENGTH+1, base);
  stripWhiteSpace(info->img);

  return 0;
}

int baseWriteData(FILE *file, artwork_info *info)
{
  /* Return error if the file or struct pointers are NULL. */
  if (!file || !info) {
    return 1;
  }

  fseek(file, 0, SEEK_END);

  fprintf(file, "%-200s",   info->title);
  fprintf(file, "%-100s",   info->type);
  fprintf(file, "%-125s",   info->author);
  fprintf(file, "%-4s",     info->year);
  fprintf(file, "%-12s",    info->value);
  fprintf(file, "%-9s",     info->img);

  fflush(file);

  return 0;
}
