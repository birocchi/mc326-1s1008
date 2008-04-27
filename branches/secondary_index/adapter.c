typedef struct {
  FILE*             base;
  PrimaryKeyIndex*  pk_index;
  SecondaryIndex*   author_index;
  SecondaryIndex*   type_index;
  SecondaryIndex*   year_index;
} Adapter;

static void
print_record (ArtworkInfo *artwork, FILE *fp_html)
{
  html_write_record_info (artwork, fp_html);
}

find()
{
  FILE *fp_html = fopen (HTMLFILE, "w");
  assert (fp_html);

  html_begin (fp_html);

  switch (find_field)
    {
      case FIELD_AUTHOR:
        mrec = secondary_index_find (db->author_index, key);
        secondary_index_foreach (db->author_index, mrec, print_record, fp_html);
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
