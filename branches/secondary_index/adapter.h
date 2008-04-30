#ifndef __ADAPTER_H
#define __ADAPTER_H

#include "base.h"
#include "memindex.h"
#include "secindex.h"

/**
 * This holds the pointers to each
 * of the indexes and database we hava.
 */
typedef struct
{
  Base * base;
  MemoryIndex * pk_index;
  SecondaryIndex * author_index;
  SecondaryIndex * type_index;
  SecondaryIndex * year_index;
} Adapter;

/**
 * Searches in the indexes
 *
 * @param db Pointer to Adapter struct
 */
void adapter_find (Adapter * db);

/**
 * Handles memory 'freedom'.
 *
 * @param db Pointer to Adapter struct
 */
void adapter_free (Adapter * db);

/**
 * Performs inserions in the database
 *
 * @param db Pointer to Adapter struct
 */
void adapter_insert (Adapter * db);

/**
 * Lists the artworks.
 *
 * @param db Pointer to Adapter struct
 */
void adapter_list (Adapter * db);

/**
 * Load the indexes from files.
 *
 * @param db Pointer to Adapter struct
 */
void adapter_load_files (Adapter * db);

/**
 * Creates a new adapter pointer.
 * In case it's the first run.
 *
 * @param db Pointer to Adapter struct
 * @ returns Pointer to Adapter
 */
Adapter * adapter_new (void);

/**
 * Removes something from the database
 *
 * @param db Pointer to Adapter struct
 */
void adapter_remove (Adapter * db);

#endif
