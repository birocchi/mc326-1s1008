#include <assert.h>
#include <stdargs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "menu.h"

typedef struct {
  Base              *base;
  PrimaryKeyIndex*  pk_index;
  SecondaryIndex*   author_index;
  SecondaryIndex*   type_index;
  SecondaryIndex*   year_index;
} Adapter;

static void
print_record (const char *name, int rrn, va_list ap)
{
  Adapter *db;
  ArtworkInfo artwork;
  FILE *html_fp;
  MemoryIndexRecord *rec;

  db = va_arg(ap, Adapter*);
  html_fp = va_arg(ap, FILE*);

  rec = memory_index_find (db->pk_index, name);
  if (rec)
    {
      fseek (db->base->fp, rec->rrn * BASE_REG_SIZE, SEEK_SET);
      base_read_artwork_record (db->base->fp, &artwork);
      html_write_record_info (&artwork, fp_html);
    }
}

void
adapter_free (Adapter *db)
{
  if (db)
    {
      base_free (db->base);
      memory_index_free (db->pk_index);
      secondary_index_free (db->author_index);
      secondary_index_free (db->type_index);
      secondary_index_free (db->year_index);
      free (db);
    }
}

void
adapter_load_files (Adapter *db)
{
  assert (db);

  if ((!fileExists (DBFILE)) || (getFileSizeWithName (DBFILE) < 1))
    {
      db->base = base_new (DBFILE, DBFILE_AVAIL, 1);
      db->pk_index = memory_index_new (PKFILE, 0);
      db->author_index = secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL. 1);
      db->type_index = secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 1);
      db->year_index = secondary_index_new (SI_YEAR, SI_YEAR_LIST, SI_YEAR_AVAIL, 1);
    }
  else
    {
      if (!fileExists (PKFILE))
        db->pk_index = pk_load_from_base (DBFILE, PKFILE);
      else
        {
          db->pk_index = memory_index_new (PKFILE, 0);
          memory_index_load_from_file (db->pk_index, PKFILE);
        }

      db->author_index = secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL. 0);
      db->type_index = secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 0);
      db->year_index = secondary_index_new (SI_YEAR, SI_YEAR_LIST, SI_YEAR_AVAIL, 0);
    }
}

Adapter *
adapter_new (void)
{
  return MEM_ALLOC (Adapter);
}

void
adapter_find (Adapter *db)
{
  ArtworkInfo artwork;
  char field;
  char key[TITLE_LENGTH+1]; /* TITLE_LENGTH is the largest of all lengths */
  FILE *fp_html;
  SecondaryIndex *secindex;
 
  fp_html = fopen (HTMLFILE, "w");
  assert (fp_html);
  html_begin (fp_html);

  field = menuMultipleAnswers ("   Opcao desejada: ", "auit");
  switch (field)
    {
      case 'a': /* Year */
        secindex = db->year_index;
        readString ("   Digite o ano para busca: ", key, YEAR_LENGTH);
        break;
      case 'i': /* Type */
        secindex = db->type_index;
        readString ("   Digite o tipo para busca: ", key, TYPE_LENGTH);
        break;
      case 't': /* Title */
        readString ("   Digite o titulo para busca: ", key, TITLE_LENGTH);
        break;
      case 'u': /* Author */
        secindex = db->author_index;
        readString ("   Digite o autor para busca: ", key, AUTHOR_LENGTH);
        break;
      default:
        break;
    }

  if (field != 't')
    {
      mrec = secondary_index_find (secindex, key);

      if (mrec)
        secondary_index_foreach (db->author_index, mrec, print_record, base, fp_html);
      else
        printf ("   Nao foi encontrada nenhuma obra.\n");
    }
  else
    {
      mrec = memory_index_find (index->pk_list, key);

      if (mrec)
        {
          fseek (db->base->fp, mrec->rrn * BASE_REG_SIZE, SEEK_SET);
          base_read_artwork_info (db->base->fp, &artwork);
          html_write_record_info (fp_html, &artwork);
        }
      else
        printf ("   Nao foi encontrada nenhuma obra.\n");
    }

  printf ("O resultado da busca foi gravado em \"%s\".\n", HTMLFILE);

  if (menuYesOrNo ("Apagar algum resultado da busca? (s)im, (n)ao? "))
    {
    }

  fclose (fp_html);
}

insert
{
  ArtworkInfo artwork;

  base_read_input(&artwork);

  if (pk_find_rrn(artwork.title) != -1) {
    base_insert(db->base, &artwork);

    pkListInsert(db->pk_index, artwork.title);

    secondary_index_insert(db->author_index, artwork.author, artwork.title);
    secondary_index_insert(db->type_index, artwork.type, artwork.title);
    secondary_index_insert(db->year_index, artwork.year, artwork.title);
  } else
    return ERR_REPEATED_PK;
}
