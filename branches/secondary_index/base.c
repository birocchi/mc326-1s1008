#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avail.h"
#include "base.h"
#include "io.h"
#include "mem.h"

void
base_free (Base *b)
{
  if (b)
    {
      avail_list_free (b->avlist);
      fclose (b->fp);
      free (b);
    }
}

/*
 * TODO: There's room for optimization here (too many disk seeks)
 */
void
base_insert (Base *base, ArtworkInfo *info)
{
  int writepos;

  assert (base != NULL && info != NULL);

  if (avail_list_is_empty (base->avlist))
    {
      fseek (base->fp, 0, SEEK_END);
      base_write_data (base->fp, info);
    }
  else
    {
      writepos = avail_list_pop (base->avlist, base->fp);
      fseek (base->fp, writepos, SEEK_SET);
      base_write_data (base->fp, info);
    }
}

Base *
base_new (const char *basename, const char *availname, int writeonly)
{
  Base *b = MEM_ALLOC (Base);

  b->avlist = avail_list_new (availname, BASE_REG_SIZE);

  if ((fileExists (basename) && (getFileSizeWithName (basename) > 0)))
    {
      b->fp = fopen (basename, "r+");
      avail_list_load (b->avlist);
    }
  else
    b->fp = fopen (basename, "w");
  assert (b->fp);

  return b;
}

void
base_remove (Base *base, int rrn)
{
  assert (base != NULL);

  fseek (base->fp, rrn * BASE_REG_SIZE, SEEK_SET);
  fprintf (base->fp, "%s", atoi (avail_list_get_tail (base->avlist)));

  avail_list_push (base->avlist, rrn);
}

/*
 * TODO: Break this into smaller functions
 */
void
base_read_input(ArtworkInfo *info)
{
  /* We use IMG_LENGTH-2 here because we exclude the first
   * two characters in the identifier (the group number)
   * since it's constant.
   */
  char img[ (IMG_LENGTH-2)+1 ];

  assert(info != NULL);

  readString("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ",
             info->title, TITLE_LENGTH);
  readString("   Por favor, digite o tipo da obra (Max: 100 caracteres): ",
             info->type, TYPE_LENGTH);
  readString("   Por favor, digite o autor da obra (Max: 125 caracteres): ",
             info->author, AUTHOR_LENGTH);

  readInt("   Por favor, digite o ano da obra (Max: 4 caracteres): ",
          info->year, YEAR_LENGTH);
  readInt("   Por favor, digite o valor da obra (Max: 12 caracteres): ",
          info->value, VALUE_LENGTH);

  while (1) {
    readString("   Por favor, digite o identificador da obra (Max: 7 caracteres): ",
               img, IMG_LENGTH-2);

    /* Validate the image identifier */
    if (!baseIsValidIdentifier(img)) {
      strncpy(info->img, GROUP_NUMBER, 2);
      strncpy(info->img+2, img, (IMG_LENGTH-2)+1);
      break;
    } else {
      printf("   Entrada invalida.");
      continue;
    }
  }
}

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

void
base_read_artwork_record (FILE *base, ArtworkInfo *info)
{
  assert ((base != NULL) && (info != NULL));

  fgets (info->title, TITLE_LENGTH+1, base);
  stripWhiteSpace (info->title);
  fgets (info->type, TYPE_LENGTH+1, base);
  stripWhiteSpace (info->type);
  fgets (info->author, AUTHOR_LENGTH+1, base);
  stripWhiteSpace (info->author);
  fgets (info->year, YEAR_LENGTH+1, base);
  stripWhiteSpace (info->year);
  fgets (info->value, VALUE_LENGTH+1, base);
  stripWhiteSpace (info->value);
  fgets (info->img, IMG_LENGTH+1, base);
  stripWhiteSpace (info->img);
}

void
base_write_data(FILE *file, ArtworkInfo *info)
{
  assert(file != NULL and info != NULL);

  fprintf(file, "%-200s",   info->title);
  fprintf(file, "%-100s",   info->type);
  fprintf(file, "%-125s",   info->author);
  fprintf(file, "%-4s",     info->year);
  fprintf(file, "%-12s",    info->value);
  fprintf(file, "%-9s",     info->img);

  fflush(file);
}
