#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adapter.h"
#include "base.h"
#include "descriptor.h"
#include "file.h"
#include "filelist.h"
#include "hash.h"
#include "html.h"
#include "io.h"
#include "libimg/libimg.h"
#include "mem.h"
#include "memindex.h"
#include "menu.h"
#include "secindex.h"

static int check_all_indexes (char *filenames[], size_t n, size_t maxhash);
static void load_files_from_base (Adapter * db);
static void print_record (char *name, int rrn, va_list ap);
static void read_secindex (Adapter * db, MemoryIndex ** mindex,
                           SecondaryIndex ** secindex, char *key);
static void secindex_insert_wrapper (char *str, va_list ap);
static void secindex_remove_wrapper (char *str, va_list ap);

/**
 * @brief Check if all index files are present and valid.
 *
 * @param filenames An array with all the file prefixes to check.
 * @param n         The number of the elements in the array.
 *
 * @retval 0 All the files are valid.
 * @retval 1 Some file is not valid.
 *
 * The file names passed in the array must be only the prefixes,
 * since the function is responsible for adding the right suffix
 * based on the number of hash files being used.
 */
static int
check_all_indexes (char *filenames[], size_t n, size_t maxhash)
{
  char *filename;
  FILE *fp;
  int retval = 0;
  unsigned int i, j;

  for (i = 0; i < n; i++)
    {
      for (j = 0; j < maxhash; j++)
        {
          filename = hash_get_filename (filenames[i], j, maxhash);

          if (!file_exists (filename))
            {
              fp = fopen (filename, "w");
              fclose (fp);

              retval = 1;
            }

          free (filename);
        }
    }

  return retval;
}

/**
 * @brief Read data from the base to all the indexes used.
 *
 * @param db The database.
 */
static void
load_files_from_base (Adapter * db)
{
  ArtworkInfo artwork;
  char *title;
  int itemcount, i;

  assert (db);

  itemcount = getFileSize (db->base->fp) / BASE_REG_SIZE;

  fseek (db->base->fp, 0, SEEK_SET);

  for (i = 0; i < itemcount; i++)
    {
      base_read_artwork_record (db->base, &artwork);

      title = str_dup (artwork.title);

      memory_index_insert (db->pk_index, title, i);
      descriptor_insert (db->desc, title,
                         CalculaDescritor (baseGetValidImagePath
                                           (artwork.img)));

      str_foreach (artwork.author, secindex_insert_wrapper,
                   db->author_index, title);
      str_foreach (artwork.title, secindex_insert_wrapper, db->title_index,
                   title);
      str_foreach (artwork.type, secindex_insert_wrapper, db->type_index,
                   title);
      str_foreach (artwork.year, secindex_insert_wrapper, db->year_index,
                   title);

      free (title);
    }
}

static void
print_record (char *name, int rrn, va_list ap)
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
  base_read_artwork_record (db->base, &artwork);
  /* Then write it to the HTML file. */
  html_write_record_info (html_fp, &artwork);
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
secindex_insert_wrapper (char *str, va_list ap)
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
secindex_remove_wrapper (char *str, va_list ap)
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
  char img[255];
  FILE *fp_html;
  MemoryIndex *mindex = NULL;
  MemoryIndexRecord *mrec = NULL;
  SecondaryIndex *secindex = NULL;

  print_search_type_menu ();
  switch (menuMultipleAnswers ("   Opcao desejada: ", "eps"))
    {
    case 'e':
      readString ("   Digite o titulo para busca: ", key, TITLE_LENGTH);
      mindex = db->pk_index;
      break;
    case 'p':
      read_secindex (db, &mindex, &secindex, key);
      break;
    case 's':
      readString ("   Digite o nome da imagem de comparacao: ", img, 255);

      if (isValidFile (img))
        descriptor_find (db->desc, img, db->pk_index, db->base->fp, 50);
      else
        printf ("   Imagem \"%s\" invalida ou nao encontrada.", img);

      return;
    }

  mrec = memory_index_find (mindex, key);
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
          base_read_artwork_record (db->base, &artwork);
          html_write_record_info (fp_html, &artwork);
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
      descriptor_free (db->desc);
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
  int baserrn;

  do
    {
      base_read_input (&artwork);

      if (memory_index_find (db->pk_index, artwork.title) == NULL)
        {
          title = str_dup (artwork.title);

          baserrn = base_insert (db->base, &artwork);

          memory_index_insert (db->pk_index, title, baserrn);
          descriptor_insert (db->desc, title,
                             CalculaDescritor (baseGetValidImagePath
                                               (artwork.img)));

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
adapter_load_files (Adapter * db)
{
  char *filenames[] = { PKFILE, SI_AUTHOR_INDEX, SI_TITLE_INDEX,
    SI_TYPE_INDEX, SI_YEAR_INDEX
  };
  char *descnames[] = { DESCFILE };
  int loadbase = 0, loadbase2 = 0;

  assert (db);

  db->base = base_new (DBFILE, DBFILE_AVAIL);
  db->pk_index = memory_index_new (PKFILE);
  db->desc = descriptor_new (DESCFILE);

  /* Check if all hash files are valid */
  loadbase = check_all_indexes (filenames, INDEX_TOTAL, INDEX_HASH_NUM);
  loadbase2 = check_all_indexes (descnames, 1, DESC_HASH_NUM);

  /* Create each secondary index, and set the overwrite_index parameter
   * based on the result of check_all_indexes, ie, overwrite if any
   * error was found. */
  db->author_index = secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST,
                                          SI_AUTHOR_AVAIL, loadbase);
  db->title_index = secondary_index_new (SI_TITLE_INDEX, SI_TITLE_LIST,
                                         SI_TITLE_AVAIL, loadbase);
  db->type_index = secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST,
                                        SI_TYPE_AVAIL, loadbase);
  db->year_index = secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST,
                                        SI_YEAR_AVAIL, loadbase);

  /* If there was something wrong, reload all data */
  if (loadbase || loadbase2)
    {
      printf ("loading\n");
      load_files_from_base (db);
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
  MemoryIndexRecord *match;

  readString ("   Digite o titulo da obra: ", key, TITLE_LENGTH);

  match = memory_index_find (db->pk_index, key);
  if (match)
    {
      fseek (db->base->fp, match->rrn * BASE_REG_SIZE, SEEK_SET);
      base_read_artwork_record (db->base, &artwork);

      title = str_dup (artwork.title);
      base_remove (db->base, match->rrn);
      memory_index_remove (db->pk_index, match->name);
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
    }
  else
    printf ("   A obra \"%s\" nao foi encontrada..\n", key);
}
