#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

char* getValidImagePath(char* s) {
  return s;
}

/*
void printArtworkRecord(artwork_info* info)
{
  if (info) {
    printf("  Titulo: %s\n", info->title);
    printf("  Tipo: %s\n", info->type);
    printf("  Autor: %s\n", info->author);
    printf("  Ano de criacao: %04d\n", info->year);
    printf("  Valor: %012d\n", info->value);
    printf("  Identificador: %s", info->img);
  }
}
*/

int readArtworkRecord(FILE *base, artwork_info *info)
{
  if (!base || !info) {
    return 1;
  }

  fgets(info->title, NAME_LENGTH, base);
  fgets(info->type, TYPE_LENGTH, base);
  fgets(info->author, AUTHOR_LENGTH, base);
  fscanf(base, "%04d", &(info->year));
  fscanf(base, "%012d", &(info->value));
  fgets(info->img, IMG_LENGTH+1, base);

  return 0;
}

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
