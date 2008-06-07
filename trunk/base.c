#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avail.h"
#include "base.h"
#include "file.h"
#include "io.h"
#include "mem.h"

void
base_free (Base * b)
{
  if (b)
    /* If b is allocated.. */
    {
      /* Free and close everyhing. */
      avail_list_free (b->avlist);
      fclose (b->fp);
      free (b);
    }
}

/* TODO: There's room for optimization here (too many disk seeks) */
void
base_insert (Base * base, ArtworkInfo * info)
{
  int writepos;

  /* Checking consistency... */
  assert (base != NULL && info != NULL);

  /* If there are no available spots, then just insert it to the end. */
  if (avail_list_is_empty (base->avlist))
    {
      fseek (base->fp, 0, SEEK_END);
      base_write_data (base->fp, info);
    }
  else
    /* In the other hand, if we have available spots, we write it
       there and remove that position from the avail list. */
    {
      writepos = avail_list_pop (base->avlist, base->fp);
      fseek (base->fp, writepos, SEEK_SET);
      base_write_data (base->fp, info);
    }
}

Base *
base_new (const char *basename, const char *availname)
{
  Base *b = MEM_ALLOC (Base);

  b->fp = fopen (basename, (isValidFile (basename) ? "r+" : "w+"));
  assert (fp);

  b->avlist = avail_list_new (availname, BASE_REG_SIZE);
  avail_list_load (b->avlist);

  return b;
}

void
base_remove (Base * base, int rrn)
{
  assert (base);

  /* Add that position to the avail list. */
  avail_list_push (base->avlist, base->fp, rrn);
}

/*
 * TODO: Break this into smaller functions
 */
void
base_read_input (ArtworkInfo * info)
{
  /* We use IMG_LENGTH-2 here because we exclude the first
   * two characters in the identifier (the group number)
   * since it's constant.
   */
  char img[(IMG_LENGTH - 2) + 1];

  assert (info != NULL);

  readString
    ("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ",
     info->title, TITLE_LENGTH);
  readString ("   Por favor, digite o tipo da obra (Max: 100 caracteres): ",
              info->type, TYPE_LENGTH);
  readString ("   Por favor, digite o autor da obra (Max: 125 caracteres): ",
              info->author, AUTHOR_LENGTH);

  readInt ("   Por favor, digite o ano da obra (Max: 4 caracteres): ",
           info->year, YEAR_LENGTH);
  readInt ("   Por favor, digite o valor da obra (Max: 12 caracteres): ",
           info->value, VALUE_LENGTH);

  while (1)
    {
      readString
        ("   Por favor, digite o identificador da obra (Max: 7 caracteres): ",
         img, IMG_LENGTH - 2);

      /* Validate the image identifier */
      if (!baseIsValidIdentifier (img))
        {
          strncpy (info->img, GROUP_NUMBER, 2);
          strncpy (info->img + 2, img, (IMG_LENGTH - 2) + 1);
          break;
        }
      else
        {
          printf ("   Entrada invalida.");
          continue;
        }
    }
}

char *
baseGetValidImagePath (const char *s)
{
  char *file;


  file = MEM_ALLOC_N (char, IMG_LENGTH + 2);

  /* Change the identifier string so it becomes the file path. */
  strncpy (file, s, 6);
  file[6] = '.';
  strncpy (file + 7, s + 6, 3);
  file[10] = '\0';

  return file;
}

int
baseIsValidIdentifier (const char *name)
{
  char *endptr;
  int i;

  /* Checking the extension, length, etc.. */

  i = strtol (name, &endptr, 10);
  if ((endptr == name) || (endptr == '\0') || strlen (endptr) != 3
      || ((strncmp (endptr, "jpg", 3)) && strncmp (endptr, "gif", 3)
          && (strncmp (endptr, "png", 3))))
    return 1;
  else
    return 0;
}

void
base_read_artwork_record (FILE * base, ArtworkInfo * info)
{
  assert ((base != NULL) && (info != NULL));

  /* Read it all... */
  fgets (info->title, TITLE_LENGTH + 1, base);
  fgets (info->type, TYPE_LENGTH + 1, base);
  fgets (info->author, AUTHOR_LENGTH + 1, base);
  fgets (info->year, YEAR_LENGTH + 1, base);
  fgets (info->value, VALUE_LENGTH + 1, base);
  fgets (info->img, IMG_LENGTH + 1, base);
  /* Strip the whitespaces from it all. */
  stripWhiteSpace (info->title);
  stripWhiteSpace (info->type);
  stripWhiteSpace (info->author);
  stripWhiteSpace (info->year);
  stripWhiteSpace (info->value);
  stripWhiteSpace (info->img);
}

void
base_write_data (FILE * file, ArtworkInfo * info)
{
  /* Consistency check.. */
  assert (file != NULL && info != NULL);

  /* Right all the fields to the file. */
  fprintf (file, "%-200s", info->title);
  fprintf (file, "%-100s", info->type);
  fprintf (file, "%-125s", info->author);
  fprintf (file, "%-4s", info->year);
  fprintf (file, "%-12s", info->value);
  fprintf (file, "%-9s", info->img);

  /* Make sure it goes to the disk. */
  fflush (file);
}
