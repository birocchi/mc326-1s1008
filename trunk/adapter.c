/* Make atoll available */
#define _XOPEN_SOURCE 600

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

/**
 * Actions that \a secindex_wrapper is allowed to perform.
 */
typedef enum
{
  WRAPPER_INSERT,
  WRAPPER_REMOVE
} SecIndexWrapperAction;

/**
 * A tuple-like data structure used by \a create_indexes_if_needed.
 * Contains a file prefix and the number of files it is split into.
 */
typedef struct
{
  char prefix[255];
  unsigned int hashcount;
} FileLoadTuple;

/**
 * @brief Checks if all indexes are valid files; if not, create them.
 *
 * @param files An array of \a FileLoadTuple structures.
 * @param count The length of the array.
 *
 * @retval 0 All indexes were present and valid.
 * @retval 1 Some indexes had do be created.
 */
static int create_indexes_if_needed (FileLoadTuple * files, size_t count);

/**
 * @brief Inserts a new artwork entry to the database's indexes.
 *
 * @param db      The \a Adapter in use.
 * @param artwork The new artwork entry.
 * @param rrn     The relative register number of the new entry in the base.
 */
static void insert_indexes (Adapter * db, ArtworkInfo * artwork, int rrn);

/**
 * @brief Loads the database's indexes with data directly from the base file.
 *
 * @param db The \a Adapter in use.
 */
static void load_files_from_base (Adapter * db);

/**
 * @brief Given a primary key, print information about a database record.
 *
 * @param name  The primary key name.
 * @param rrn   Unused.
 * @param ap    Must be \a Adapter* and \a FILE* -- the database
 *              and the HTML output.
 *
 * This function is a wrapper for a \a secondary_index_foreach call.
 */
static void print_record (char *name, int rrn, va_list ap);

/**
 * @brief Reads the user's index choice for a partial search.
 *
 * @param db        The \a Adapter in use.
 * @param mindex    The \a MemoryIndex that will be used.
 * @param secindex  The \a SecondaryIndex that will be used.
 * @param key       The value the user will search for.
 */
static void read_secindex (Adapter * db, MemoryIndex ** mindex,
                           SecondaryIndex ** secindex, char *key);

/**
 * @brief Inserts or removes an entry from a secondary index.
 *
 * @param str The entry to be inserted.
 * @param ap  Must be a \a SecondaryIndex*, a \a char*
 *            and a \a SecondaryIndexWrapperAction.
 */
static void secindex_wrapper (char *str, va_list ap);

static int
create_indexes_if_needed (FileLoadTuple * files, size_t count)
{
  int retval = 0;
  unsigned int i;

  for (i = 0; i < count; i++)
    {
      if (file_create_if_needed (files[i].prefix, files[i].hashcount))
        retval = 1;
    }

  return retval;
}

static void
insert_indexes (Adapter * db, ArtworkInfo * artwork, int rrn)
{
  char *imgpath = base_get_image_path (artwork->img),
       *pkname = str_dup (artwork->title);

  memory_index_insert (db->pk_index, pkname, rrn);
  descriptor_insert (db->desc, pkname, CalculaDescritor (imgpath));

  str_foreach (artwork->author, secindex_wrapper,
               db->author_index, pkname, WRAPPER_INSERT);
  str_foreach (artwork->title, secindex_wrapper,
               db->title_index, pkname, WRAPPER_INSERT);
  str_foreach (artwork->type, secindex_wrapper, db->type_index,
               pkname, WRAPPER_INSERT);
  str_foreach (artwork->year, secindex_wrapper, db->year_index,
               pkname, WRAPPER_INSERT);

  free (imgpath);
  free (pkname);
}

static void
load_files_from_base (Adapter * db)
{
  ArtworkInfo artwork;
  int itemcount, i;

  assert (db);

  fseek (db->base->fp, 0, SEEK_SET);

  itemcount = file_get_size (db->base->fp) / BASE_REG_SIZE;

  for (i = 0; i < itemcount; i++)
    {
      base_read_artwork_record (db->base, &artwork);
      insert_indexes (db, &artwork, i);
    }
}

static void
print_record (char *name, int rrn, va_list ap)
{
  Adapter *db;
  FILE *html_fp;
  MemoryIndexRecord *rec;

  db = va_arg (ap, Adapter *);
  html_fp = va_arg (ap, FILE *);

  rec = memory_index_find (db->pk_index, name);
  assert (rec);

  base_read_artwork_write_html (db->base, html_fp, rec->rrn);
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
secindex_wrapper (char *str, va_list ap)
{
  char *pk_value = NULL;
  SecondaryIndex *secindex = NULL;
  SecIndexWrapperAction action;

  secindex = va_arg (ap, SecondaryIndex *);
  pk_value = va_arg (ap, char *);
  action = va_arg (ap, SecIndexWrapperAction);

  assert (secindex && pk_value);

  switch (action)
    {
    case WRAPPER_INSERT:
      secondary_index_insert (secindex, str, pk_value);
      break;
    case WRAPPER_REMOVE:
      secondary_index_remove (secindex, str, pk_value);
      break;
    }
}

void
adapter_find (Adapter * db)
{
  char key[255];                /* Maximum file name length in many file
                                   systems (can also hold any entry field) */
  char search_type;
  char strmaxresults[11];       /* In a 32-bit system, UINT_MAX has 10 digits */
  FILE *html_fp;
  int has_matches, i;
  MemoryIndex *mindex = NULL;
  MemoryIndexRecord *mrec = NULL;
  SecondaryIndex *secindex = NULL;
  SimilarityList *simlist = NULL;
  unsigned int maxresults;

  /* User interface */
  print_search_type_menu ();
  search_type = menuMultipleAnswers ("   Opcao desejada: ", "eps");
  switch (search_type)
    {
    case 'e':
      read_string ("   Digite o titulo para busca: ", key, TITLE_LENGTH);
      mrec = memory_index_find (db->pk_index, key);
      break;
    case 'p':
      read_secindex (db, &mindex, &secindex, key);
      mrec = memory_index_find (mindex, key);
      break;
    case 's':
      read_string ("   Digite o nome da imagem de comparacao: ", key, 255);

      if (!file_is_valid_image (key))
        {
          printf ("\n   Imagem \"%s\" invalida ou nao encontrada.\n", key);
          return;
        }

      read_int ("   Digite o numero maximo de resultados desejados: ",
                strmaxresults, 10);

      simlist = descriptor_find (db->desc, db->base, db->pk_index, key);

      break;
    }

  /* Search code */
  has_matches = ((mrec != NULL) || ((simlist != NULL) && (simlist->regnum > 0)));

  if (has_matches)
    {
      html_fp = fopen (HTMLFILE, "w");
      html_begin (html_fp);

      switch (search_type)
        {
        case 'e': /* Exact search */
          base_read_artwork_write_html (db->base, html_fp, mrec->rrn);
          break;
        case 'p': /* Partial search */
          secondary_index_foreach (secindex, mrec, print_record, db, html_fp);
          break;
        case 's': /* Similarity search */
          fprintf (html_fp, "<tr><td><img src=\"%s\"></td><td></td></tr>\n\n",
                   key);

          maxresults = atoll (strmaxresults);

          for (i = 0; (i < maxresults) && (i < simlist->regnum); i++)
            base_read_artwork_write_html (db->base, html_fp, simlist->list[i].rrn);

          simlist_free (simlist);

          break;
        }

      html_end (html_fp);
      fclose (html_fp);

      printf ("\n   O resultado da busca por \"%s\" foi gravado em \"%s\".\n",
              key, HTMLFILE);

      if (menuYesOrNo ("\n   Apagar algum resultado? (s)im, (n)ao? "))
        adapter_remove (db);
    }
  else
    printf ("\n   Nao foi encontrada nenhuma obra.\n");
}

void
adapter_free (Adapter * db)
{
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
  int baserrn;

  do
    {
      base_read_input (&artwork);

      if (memory_index_find (db->pk_index, artwork.title) == NULL)
        {
          baserrn = base_insert (db->base, &artwork);
          insert_indexes (db, &artwork, baserrn);
        }
      else
        printf ("   Ja existe uma obra com titulo \"%s\".\n", artwork.title);
    }
  while (menuYesOrNo ("\nDeseja inserir mais uma entrada? (s)im, (n)ao? "));
}

void
adapter_load_files (Adapter * db)
{
  int found_invalid;
  FileLoadTuple files[] = {
    {PKFILE, INDEX_HASH_NUM},
    {SI_AUTHOR_INDEX, INDEX_HASH_NUM},
    {SI_TITLE_INDEX, INDEX_HASH_NUM},
    {SI_TYPE_INDEX, INDEX_HASH_NUM},
    {SI_YEAR_INDEX, INDEX_HASH_NUM},
    {DESCFILE, DESC_HASH_NUM}
  };

  assert (db);

  found_invalid = create_indexes_if_needed (files, INDEX_TOTAL);

  db->base = base_new (DBFILE, DBFILE_AVAIL);
  db->pk_index = memory_index_new (PKFILE);
  db->desc = descriptor_new (DESCFILE);
  db->author_index =
    secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL,
                         found_invalid);
  db->title_index =
    secondary_index_new (SI_TITLE_INDEX, SI_TITLE_LIST, SI_TITLE_AVAIL,
                         found_invalid);
  db->type_index =
    secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL,
                         found_invalid);
  db->year_index =
    secondary_index_new (SI_YEAR_INDEX, SI_YEAR_LIST, SI_YEAR_AVAIL,
                         found_invalid);

  if (found_invalid)
    {
      printf ("   Loading data for the first time...\n");
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

  read_string ("   Digite o titulo da obra: ", key, TITLE_LENGTH);

  match = memory_index_find (db->pk_index, key);
  if (match)
    {
      base_read_artwork_record_with_rrn (db->base, &artwork, match->rrn);

      title = str_dup (artwork.title);

      base_remove (db->base, match->rrn);
      memory_index_remove (db->pk_index, match->name);
      str_foreach (artwork.author, secindex_wrapper,
                   db->author_index, title, WRAPPER_REMOVE);
      str_foreach (artwork.title, secindex_wrapper,
                   db->title_index, title, WRAPPER_REMOVE);
      str_foreach (artwork.type, secindex_wrapper, db->type_index,
                   title, WRAPPER_REMOVE);
      str_foreach (artwork.year, secindex_wrapper, db->year_index,
                   title, WRAPPER_REMOVE);

      printf ("   Obra \"%s\" removida com sucesso.\n", title);

      free (title);
    }
  else
    printf ("   A obra \"%s\" nao foi encontrada..\n", key);
}
