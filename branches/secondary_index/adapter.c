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
#include "secindex.h"

/* We use this to know which load base we are gonna use. */
/* Binary & is faster! */
enum
{
  LOAD_BASE_PK = 1 << 0,
  LOAD_BASE_AUTHOR = 1 << 1,
  LOAD_BASE_TITLE = 1 << 2,
  LOAD_BASE_TYPE = 1 << 3,
  LOAD_BASE_YEAR = 1 << 4
};

static void
load_files_from_base (Adapter * db, int loadflags)
{
  ArtworkInfo artwork;
  char *title;
  FILE *base = db->base->fp;
  int i, baselen;

  /* If we had any problems.. */
  if ((!base) || (!loadflags))
    return;

  /* How many registers we have in the database. */
  baselen = getFileSize (base) / BASE_REG_SIZE;

  /* Each time we shift LOAD_BASE and we then load something else. */
  if (loadflags & LOAD_BASE_PK)
    db->pk_index = memory_index_new (PKFILE, 0);
  if (loadflags & LOAD_BASE_AUTHOR)
    db->author_index =
      secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL,
                           1);
  if (loadflags & LOAD_BASE_TITLE)
    db->title_index =
      secondary_index_new (SI_TITLE_INDEX, SI_TITLE_LIST, SI_TITLE_AVAIL, 1);
  if (loadflags & LOAD_BASE_TYPE)
    db->type_index =
      secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 1);
  if (loadflags & LOAD_BASE_YEAR)
    db->year_index =
      secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST, SI_YEAR_AVAIL, 1);

  fseek (base, 0, SEEK_SET);

  /* We then read each register from the database. */
  for (i = 0; i < baselen; i++)
    {
      base_read_artwork_record (base, &artwork);

      title = str_dup (artwork.title);

      if (loadflags & LOAD_BASE_PK)
        memory_index_insert (db->pk_index, title, db->pk_index->regnum);

      if (loadflags & LOAD_BASE_AUTHOR)
        str_foreach (artwork.author, secindex_insert_wrapper,
                     db->author_index, title);
      if (loadflags & LOAD_BASE_TITLE)
        str_foreach (artwork.title, secindex_insert_wrapper, db->title_index,
                     title);
      if (loadflags & LOAD_BASE_TYPE)
        str_foreach (artwork.type, secindex_insert_wrapper, db->type_index,
                     title);
      if (loadflags & LOAD_BASE_YEAR)
        str_foreach (artwork.year, secindex_insert_wrapper, db->year_index,
                     title);

      free (title);
    }
}

static void
print_record (const char *name, int rrn, va_list ap)
{
  Adapter *db;
  ArtworkInfo artwork;
  FILE *html_fp;
  MemoryIndexRecord *rec;

  db = va_arg (ap, Adapter *);
  html_fp = va_arg (ap, FILE *);

  /* We search for it in the indexes. */
  rec = memory_index_find (db->pk_index, name);
  assert (rec);

  /* Go to that register position in the database. */
  fseek (db->base->fp, rec->rrn * BASE_REG_SIZE, SEEK_SET);
  /* Read it. */
  base_read_artwork_record (db->base->fp, &artwork);
  /* Then write it to the HTML file. */
  html_write_record_info (html_fp, rec->rrn, &artwork);
}

static void
read_secindex (Adapter * db, MemoryIndex ** mindex,
               SecondaryIndex ** secindex, char *key)
{
  printConsultMenu ();

  switch (menuMultipleAnswers ("   Opcao desejada: ", "auit"))
    {
    case 'a':                  /* Year */
      *secindex = db->year_index;
      *mindex = (*secindex)->record_list;
      read_word ("   Digite o ano para busca: ", key, YEAR_LENGTH);
      break;
    case 'i':                  /* Type */
      *secindex = db->type_index;
      *mindex = (*secindex)->record_list;
      read_word ("   Digite o tipo para busca: ", key, TYPE_LENGTH);
      break;
    case 't':                  /* Title */
      *secindex = db->title_index;
      *mindex = (*secindex)->record_list;
      read_word ("   Digite o titulo para busca: ", key, TITLE_LENGTH);
      break;
    case 'u':                  /* Author */
      *secindex = db->author_index;
      *mindex = (*secindex)->record_list;
      read_word ("   Digite o autor para busca: ", key, AUTHOR_LENGTH);
      break;
    }
}

static void
secindex_insert_wrapper (const char *str, va_list ap)
{
  char *pk_value = NULL;
  SecondaryIndex *secindex = NULL;

  secindex = va_arg (ap, SecondaryIndex *);
  pk_value = va_arg (ap, char *);

  assert (secindex);
  assert (pk_value);

  secondary_index_insert (secindex, str, pk_value);
}

static void
secindex_remove_wrapper (const char *str, va_list ap)
{
  char *pk_value = NULL;
  SecondaryIndex *secindex = NULL;

  secindex = va_arg (ap, SecondaryIndex *);
  pk_value = va_arg (ap, char *);

  assert (secindex);
  assert (pk_value);

  secondary_index_remove (secindex, str, pk_value);
}

void
adapter_find (Adapter * db)
{
  ArtworkInfo artwork;
  char key[TITLE_LENGTH + 1];   /* TITLE_LENGTH is the largest of all lengths */
  FILE *fp_html;
  MemoryIndex *mindex = NULL;
  MemoryIndexRecord *mrec = NULL;
  SecondaryIndex *secindex = NULL;

  print_search_type_menu ();
  switch (menuMultipleAnswers ("   Opcao desejada: ", "ep"))
    {
    case 'e':
      readString ("   Digite o titulo para busca: ", key, TITLE_LENGTH);
      mindex = db->pk_index;
      break;
    case 'p':
      read_secindex (db, &mindex, &secindex, key);
      break;
    }

  mrec = memory_index_find (mindex, key);
  /* If it found any artworks. */
  if (mrec)
    {
      fp_html = fopen (HTMLFILE, "w");
      assert (fp_html);
      html_begin (fp_html);

      if (secindex)
        secondary_index_foreach (secindex, mrec, print_record, db, fp_html);
      else
        {
          fseek (db->base->fp, mrec->rrn * BASE_REG_SIZE, SEEK_SET);
          base_read_artwork_record (db->base->fp, &artwork);
          html_write_record_info (fp_html, mrec->rrn, &artwork);
        }

      printf ("   O resultado da busca por \"%s\" foi gravado em \"%s\".\n",
              key, HTMLFILE);

      html_end (fp_html);
      fclose (fp_html);

      /* User might want to delete any. */
      if (menuYesOrNo ("   Apagar algum resultado da busca? (s)im, (n)ao? "))
        adapter_remove (db);
    }
  else
    printf ("   Nao foi encontrada nenhuma obra.");
}

void
adapter_free (Adapter * db)
{
  /* Basically frees any memory allocated. */
  if (db)
    {
      base_free (db->base);
      memory_index_free (db->pk_index);
      secondary_index_free (db->author_index);
      secondary_index_free (db->title_index);
      secondary_index_free (db->type_index);
      secondary_index_free (db->year_index);
      free (db);
    }
}

void
adapter_insert (Adapter * db)
{
  ArtworkInfo artwork;
  char *title;

  do
    {
      base_read_input (&artwork);

      if (memory_index_find (db->pk_index, artwork.title) == NULL)
        {
          title = str_dup (artwork.title);

          memory_index_insert (db->pk_index, title,
                               (avail_list_is_empty (db->base->avlist) ? db->
                                pk_index->regnum : avail_list_get_tail (db->
                                                                        base->
                                                                        avlist)));

          base_insert (db->base, &artwork);

          str_foreach (artwork.author, secindex_insert_wrapper,
                       db->author_index, title);
          str_foreach (artwork.title, secindex_insert_wrapper,
                       db->title_index, title);
          str_foreach (artwork.type, secindex_insert_wrapper, db->type_index,
                       title);
          str_foreach (artwork.year, secindex_insert_wrapper, db->year_index,
                       title);

          free (title);
        }
      else
        printf ("   Ja existe uma obra com titulo \"%s\".\n", artwork.title);
    }
  while (menuYesOrNo ("\nDeseja inserir mais uma entrada? (s)im, (n)ao? "));
}

void
adapter_list (Adapter * db)
{
  ArtworkInfo artwork;
  FILE *fp_html;
  int i;

  if (memory_index_is_empty (db->pk_index))
    printf ("   O catalogo ainda nao possui obras.\n");
  else
    {
      fp_html = fopen (HTMLFILE, "w");
      assert (fp_html);

      html_begin (fp_html);

      for (i = 0; i < db->pk_index->regnum; i++)
        {
          fseek (db->base->fp, (db->pk_index->reclist[i].rrn) * BASE_REG_SIZE,
                 SEEK_SET);
          base_read_artwork_record (db->base->fp, &artwork);
          html_write_record_info (fp_html, db->pk_index->reclist[i].rrn,
                                  &artwork);
        }

      html_end (fp_html);
      fclose (fp_html);

      printf ("   Lista \"%s\" gerada com sucesso.\n", HTMLFILE);
    }
}

void
adapter_load_files (Adapter * db)
{
  int loadbase = 0;
  assert (db);

  if (!isValidFile (DBFILE))
    {
      db->base = base_new (DBFILE, DBFILE_AVAIL, 1);
      db->pk_index = memory_index_new (PKFILE, 0);
      db->author_index =
        secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL,
                             1);
      db->title_index =
        secondary_index_new (SI_TITLE_INDEX, SI_TITLE_LIST, SI_TITLE_AVAIL,
                             1);
      db->type_index =
        secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 1);
      db->year_index =
        secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST, SI_YEAR_AVAIL, 1);
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
        db->author_index =
          secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST,
                               SI_AUTHOR_AVAIL, 0);

      if (!isValidFile (SI_TITLE_INDEX) || !isValidFile (SI_TITLE_LIST))
        loadbase |= LOAD_BASE_TITLE;
      else
        db->title_index =
          secondary_index_new (SI_TITLE_INDEX, SI_TITLE_LIST, SI_TITLE_AVAIL,
                               0);

      if (!isValidFile (SI_TYPE_INDEX) || !isValidFile (SI_TYPE_LIST))
        loadbase |= LOAD_BASE_TYPE;
      else
        db->type_index =
          secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL, 0);

      if (!isValidFile (SI_YEAR_INDEX) || !isValidFile (SI_YEAR_LIST))
        loadbase |= LOAD_BASE_YEAR;
      else
        db->year_index =
          secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST, SI_YEAR_AVAIL, 0);

      load_files_from_base (db, loadbase);
    }
}

Adapter *
adapter_new (void)
{
  return MEM_ALLOC (Adapter);
}

void
adapter_remove (Adapter * db)
{
  ArtworkInfo artwork;
  char key[TITLE_LENGTH + 1], *title;
  int rrn;

  while (1)
    {
      readInt ("   Digite o NRR da obra: ", key, TITLE_LENGTH);
      rrn = atoi (key);

      if (rrn < db->pk_index->regnum)   /* Check if the RRN is valid */
        {
          fseek (db->base->fp, rrn * BASE_REG_SIZE, SEEK_SET);
          base_read_artwork_record (db->base->fp, &artwork);

          title = str_dup (artwork.title);

          base_remove (db->base, rrn);
          memory_index_remove (db->pk_index, rrn);
          str_foreach (artwork.author, secindex_remove_wrapper,
                       db->author_index, title);
          str_foreach (artwork.title, secindex_remove_wrapper,
                       db->title_index, title);
          str_foreach (artwork.type, secindex_remove_wrapper, db->type_index,
                       title);
          str_foreach (artwork.year, secindex_remove_wrapper, db->year_index,
                       title);

          printf ("   Obra \"%s\" removida com sucesso.\n", title);

          free (title);

          break;
        }
      else
        printf ("   O NRR digitado e invalido.\n");
    }
}
