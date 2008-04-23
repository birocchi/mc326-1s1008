typedef struct {
  FILE*             base;
  PrimaryKeyIndex*  pk_index;
  SecondaryIndex*   author_index;
  SecondaryIndex*   type_index;
  SecondaryIndex*   year_index;
} Adapter;

insert
{
  artwork_info artwork;

  base_read_input(&artwork);

  if (pk_find_rrn(artwork.title) != -1) {
    base_insert(db->base, artwork);

    pk_insert(db->pk_index, base);

    secondary_index_insert(db->author_index, artwork.author, artwork.title);
    secondary_index_insert(db->type_index, artwork.type, artwork.title);
    secondary_index_insert(db->year_index, artwork.year, artwork.title);
  } else
    return ERR_REPEATED_PK;
}
