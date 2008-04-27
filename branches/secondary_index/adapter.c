#include <stdargs.h>
#include "base.h"

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

Adapter *
adapter_new (void)
{
  Adapter *db = MEM_ALLOC (Adapter);

  return db;
}

void
adapter_load_files (Adapter *db)
{
  assert (db);

  if ((!fileExists (DBFILE)) || (getFileSizeWithName (DBFILE) < 1))
    {
      db->base = base_new (DBFILE, DBFILE_AVAIL);
      db->pk_index = memory_index_new (PKFILE, 0);
      db->author_index = secondary_index_new (SI_AUTHOR_INDEX, SI_AUTHOR_LIST, SI_AUTHOR_AVAIL);
      db->type_index = secondary_index_new (SI_TYPE_INDEX, SI_TYPE_LIST, SI_TYPE_AVAIL);
      db->year_index = secondary_index_new (SI_YEAR, SI_YEAR_LIST, SI_YEAR_AVAIL);
    }
  else
    {
    }
}

find()
{
  FILE *fp_html;
 
  fp_html = fopen (HTMLFILE, "w");
  assert (fp_html);
  html_begin (fp_html);

  switch (find_field)
    {
      case FIELD_AUTHOR:
        mrec = secondary_index_find (db->author_index, key);
        secondary_index_foreach (db->author_index, mrec, print_record, base, fp_html);
        break;
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
