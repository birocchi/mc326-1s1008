typedef struct {
  artwork_info      base;
  PrimaryKeyIndex*  pk_index;
  SecondaryIndex*   author_index;
  SecondaryIndex*   type_index;
  SecondaryIndex*   year_index;
} Adapter;

insert
{
  /*
   * 1. base_input_data
   * 2. pk_find_data(base.title) == false
   * 3. pk_insert(base.all)
   * 4. author_index.insert
   * 5. type_index.insert
   * 6. year_index.insert
   *
   * Isso inclui mover readData pra base.c e transformar em base_input_data
   */
}
