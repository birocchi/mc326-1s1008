#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adapter.h"
#include "base.h"
#include "file.h"
#include "filelist.h"
#include "html.h"
#include "io.h"
#include "mem.h"
#include "memindex.h"
#include "menu.h"
#include "pk.h"
#include "secindex.h"

enum {
  LOAD_BASE_PK      = 1 << 0,
  LOAD_BASE_AUTHOR  = 1 << 1,
  LOAD_BASE_TYPE    = 1 << 2,
  LOAD_BASE_YEAR    = 1 << 3
};

static void
load_files_from_base (Adapter *db, int flags)
{
  ArtworkInfo artwork;
  FILE *base = db->base->fp;
  int i, baselen;

  if ((!base) || (!flags))
    return;

  baselen = getFileSize (base) / BASE_REG_SIZE;

  if (flags & LOAD_BASE_PK)
    db->pk_index = memory_index_new (PKFILE, 0);
  if (flags & LOAD_BASE_AUTHOR)
    db->author_index = secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL, 1);
  if (flags & LOAD_BASE_TYPE)
    db->type_index = secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 1);
  if (flags & LOAD_BASE_YEAR)
    db->year_index = secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST, SI_YEAR_AVAIL, 1);

  fseek (base, 0, SEEK_SET);

  for (i = 0; i < baselen; i++)
    {
      base_read_artwork_record (base, &artwork);

      if (flags & LOAD_BASE_PK)
        memory_index_insert (db->pk_index, artwork.title, db->pk_index->regnum);
      if (flags & LOAD_BASE_AUTHOR)
        secondary_index_insert (db->author_index, artwork.author, artwork.title);
      if (flags & LOAD_BASE_TYPE)
        secondary_index_insert (db->type_index, artwork.type, artwork.title);
      if (flags & LOAD_BASE_YEAR)
        secondary_index_insert (db->year_index, artwork.year, artwork.title);
    }
}

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
      html_write_record_info (html_fp, &artwork);
    }
}

void
adapter_find (Adapter *db)
{
  ArtworkInfo artwork;
  char field;
  char key[TITLE_LENGTH+1]; /* TITLE_LENGTH is the largest of all lengths */
  FILE *fp_html;
  MemoryIndexRecord *mrec;
  SecondaryIndex *secindex;
 
  fp_html = fopen (HTMLFILE, "w");
  assert (fp_html);
  html_begin (fp_html);

  printConsultMenu();

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
      mrec = memory_index_find (secindex->record_list, key);

      if (mrec)
        {
          secondary_index_foreach (secindex, mrec, print_record, db, fp_html);
          printf ("   O resultado da busca foi gravado em \"%s\".\n", HTMLFILE);
        }
      else
        printf ("   Nao foi encontrada nenhuma obra.\n");
    }
  else
    {
      mrec = memory_index_find (db->pk_index, key);

      if (mrec)
        {
          fseek (db->base->fp, mrec->rrn * BASE_REG_SIZE, SEEK_SET);
          base_read_artwork_record (db->base->fp, &artwork);
          html_write_record_info (fp_html, &artwork);
          printf ("   O resultado da busca foi gravado em \"%s\".\n", HTMLFILE);
        }
      else
        printf ("   Nao foi encontrada nenhuma obra.\n");
    }

  fclose (fp_html);

  if (menuYesOrNo ("   Apagar algum resultado da busca? (s)im, (n)ao? "))
    adapter_remove (db);
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
adapter_insert (Adapter *db)
{
  ArtworkInfo artwork;

  do
    {
      base_read_input(&artwork);

      if (memory_index_find (db->pk_index, artwork.title) == NULL)
        {
          base_insert(db->base, &artwork);

          memory_index_insert (db->pk_index, artwork.title, db->pk_index->regnum);

          secondary_index_insert(db->author_index, artwork.author, artwork.title);
          secondary_index_insert(db->type_index, artwork.type, artwork.title);
          secondary_index_insert(db->year_index, artwork.year, artwork.title);
        }
      else
        printf ("   Ja existe uma obra com titulo \"%s\".\n", artwork.title);
    } while (menuYesOrNo("\nDeseja inserir mais uma entrada? (s)im, (n)ao? "));
}

void
adapter_list (Adapter *db)
{
  ArtworkInfo artwork;
  FILE *fp_html;
  int i;

  if (memory_index_is_empty (db->pk_index))
    printf("   O catalogo ainda nao possui obras.\n");
  else
    {
      fp_html = fopen (HTMLFILE, "w");
      assert (fp_html);

      html_begin (fp_html);

      fseek(db->base->fp, 0, SEEK_SET);
      for (i = 0; i < db->pk_index->regnum; i++) {
        fseek (db->base->fp, (db->pk_index->reclist[i].rrn) * BASE_REG_SIZE, SEEK_SET);
        base_read_artwork_record (db->base->fp, &artwork);
        html_write_record_info (fp_html, &artwork);
      }

      html_end (fp_html);
      fclose (fp_html);

      printf ("   Lista \"%s\" gerada com sucesso.\n", HTMLFILE);
    }
}

void
adapter_load_files (Adapter *db)
{
  int loadbase = 0;
  assert (db);

  if (!isValidFile (DBFILE))
    {
      db->base = base_new (DBFILE, DBFILE_AVAIL, 1);
      db->pk_index = memory_index_new (PKFILE, 0);
      db->author_index = secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL, 1);
      db->type_index = secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 1);
      db->year_index = secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST, SI_YEAR_AVAIL, 1);
    }
  else
    {
      db->base = base_new (DBFILE, DBFILE_AVAIL, 0);

      if (!isValidFile (PKFILE))
        loadbase |= LOAD_BASE_PK;
      else
        {
          db->pk_index = memory_index_new (PKFILE, 0);
          memory_index_load_from_file (db->pk_index, PKFILE);
        }

      if (!isValidFile (SI_AUTHOR_INDEX) || !isValidFile (SI_AUTHOR_LIST))
        loadbase |= LOAD_BASE_AUTHOR;
      else
        db->author_index = secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL, 0);

      if (!isValidFile (SI_TYPE_INDEX) || !isValidFile (SI_TYPE_LIST))
        loadbase |= LOAD_BASE_TYPE;
      else
        db->type_index = secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 0);

      if (!isValidFile (SI_YEAR_INDEX) || !isValidFile (SI_YEAR_LIST))
        loadbase |= LOAD_BASE_YEAR;
      else
        db->year_index = secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST, SI_YEAR_AVAIL, 0);

      load_files_from_base (db, loadbase);
    }
}

Adapter *
adapter_new (void)
{
  return MEM_ALLOC (Adapter);
}

void
adapter_remove (Adapter *db)
{
  ArtworkInfo artwork;
  MemoryIndexRecord *mrec;
  char key[TITLE_LENGTH+1];

  readString ("   Digite o titulo da obra: ", key, TITLE_LENGTH);

  mrec = memory_index_find (db->pk_index, key);
  if (mrec)
    {
      fseek (db->base->fp, mrec->rrn * BASE_REG_SIZE, SEEK_SET);
      base_read_artwork_record (db->base->fp, &artwork);

      base_remove (db->base, mrec->rrn);
      memory_index_remove (db->pk_index, mrec);
      secondary_index_remove (db->author_index, artwork.author, key);
      secondary_index_remove (db->type_index, artwork.author, key);
      secondary_index_remove (db->year_index, artwork.author, key);
    }
  else
    printf ("   Nao ha nenhuma obra com titulo \"%s\".\n", key);
}
