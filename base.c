#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "avail.h"
#include "base.h"
#include "file.h"
#include "html.h"
#include "io.h"
#include "mem.h"

/**
 * Checks if the image identifier is valid.
 * This time we use strtol to get past the digits and also
 * check the file extension.
 *
 * @param name The image identifier string.
 * 
 * @return Returns 1 on error and 0 for OK.
 */
static int is_valid_identifier (const char *name);

static int
is_valid_identifier (const char *name)
{
  char *endptr;
  int i;

  /* Checking the extension, length, etc.. */
  i = strtol (name, &endptr, 10);
  if ((endptr == name) || (endptr == '\0') || strlen (endptr) != 3
      || ((strcasecmp (endptr, "jpg")) && strcasecmp (endptr, "gif")
          && (strcasecmp (endptr, "png"))))
    return 1;
  else
    return 0;
}

void
base_free (Base * b)
{
  if (b)
    {
      avail_list_free (b->avlist);
      fclose (b->fp);
      free (b);
    }
}

/* TODO: There's room for optimization here (too many disk seeks) */
int
base_insert (Base * base, ArtworkInfo * info)
{
  int newrrn;

  assert (base && info);

  /* If there are no available spots, then just insert it to the end. */
  if (avail_list_is_empty (base->avlist))
    {
      fseek (base->fp, 0, SEEK_END);
      newrrn = ftell (base->fp) / BASE_REG_SIZE;
    }
  else
    /* On the other hand, if we have available spots, we write it
       there and remove that position from the avail list. */
    {
      newrrn = avail_list_pop (base->avlist, base->fp);
      fseek (base->fp, newrrn * BASE_REG_SIZE, SEEK_SET);
    }

  base_write_data (base->fp, info);

  return newrrn;
}

Base *
base_new (const char *basename, const char *availname)
{
  Base *b = MEM_ALLOC (Base);

  b->fp = fopen (basename, (file_is_valid (basename) ? "r+" : "w+"));
  assert (b->fp);

  b->avlist = avail_list_new (availname);
  avail_list_load (b->avlist);

  return b;
}

void
base_remove (Base * base, int rrn)
{
  assert (base);

  avail_list_push (base->avlist, base->fp, rrn * BASE_REG_SIZE);
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

  read_string
    ("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ",
     info->title, TITLE_LENGTH);
  read_string ("   Por favor, digite o tipo da obra (Max: 100 caracteres): ",
               info->type, TYPE_LENGTH);
  read_string ("   Por favor, digite o autor da obra (Max: 125 caracteres): ",
               info->author, AUTHOR_LENGTH);

  read_int ("   Por favor, digite o ano da obra (Max: 4 caracteres): ",
            info->year, YEAR_LENGTH);
  read_int ("   Por favor, digite o valor da obra (Max: 12 caracteres): ",
            info->value, VALUE_LENGTH);

  while (1)
    {
      read_string
        ("   Por favor, digite o identificador da obra (Max: 7 caracteres): ",
         img, IMG_LENGTH - 2);

      /* Validate the image identifier */
      if (!is_valid_identifier (img))
        {
          strncpy (info->img, GROUP_NUMBER, 2);
          strncpy (info->img + 2, img, (IMG_LENGTH - 2) + 1);

          if (!is_valid_identifier (info->img))
            {
              printf ("   Imagem \"%s\" nao encontrada.\n", info->img);
              continue;
            }

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
base_get_image_path (const char *s)
{
  char *basename;
  char *fullpath;

  basename = MEM_ALLOC_N (char, IMG_LENGTH + 2);

  /* Make '012426png' become '012426.png' */
  strncpy (basename, s, 6);
  basename[6] = '.';
  strncpy (basename + 7, s + 6, 3);
  basename[10] = '\0';

  fullpath = str_join ("img/", basename);

  free (basename);

  return fullpath;
}

void
base_read_artwork_record (Base * base, ArtworkInfo * info)
{
  assert (base && info);

  fgets (info->title, TITLE_LENGTH + 1, base->fp);
  fgets (info->type, TYPE_LENGTH + 1, base->fp);
  fgets (info->author, AUTHOR_LENGTH + 1, base->fp);
  fgets (info->year, YEAR_LENGTH + 1, base->fp);
  fgets (info->value, VALUE_LENGTH + 1, base->fp);
  fgets (info->img, IMG_LENGTH + 1, base->fp);

  stripWhiteSpace (info->title);
  stripWhiteSpace (info->type);
  stripWhiteSpace (info->author);
  stripWhiteSpace (info->year);
  stripWhiteSpace (info->value);
  stripWhiteSpace (info->img);
}

void
base_read_artwork_record_with_rrn (Base * base, ArtworkInfo * info, int rrn)
{
  fseek (base->fp, rrn * BASE_REG_SIZE, SEEK_SET);
  base_read_artwork_record (base, info);
}

void
base_read_artwork_write_html (Base * base, FILE * html_fp, int rrn)
{
  ArtworkInfo artwork;

  base_read_artwork_record_with_rrn (base, &artwork, rrn);
  html_write_record_info (html_fp, &artwork);
}

void
base_write_data (FILE * file, ArtworkInfo * info)
{
  assert (file && info);

  fprintf (file, "%-200s", info->title);
  fprintf (file, "%-100s", info->type);
  fprintf (file, "%-125s", info->author);
  fprintf (file, "%-4s", info->year);
  fprintf (file, "%-12s", info->value);
  fprintf (file, "%-9s", info->img);

  /* Make sure it goes to the disk. */
  fflush (file);
}
