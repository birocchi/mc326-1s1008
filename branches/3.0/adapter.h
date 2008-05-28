#ifndef __ADAPTER_H
#define __ADAPTER_H

#include "base.h"
#include "memindex.h"
#include "secindex.h"

/**
 * This holds the pointers to each
 * of the indexes and database we have.
 */
typedef struct
{
  Base *base; /**< The base used to hold data. */
  MemoryIndex *pk_index; /**< The primary keys index. */
  SecondaryIndex *author_index; /**< The author secondary index. */
  SecondaryIndex *title_index; /**< The title secondary index. */
  SecondaryIndex *type_index; /**< The type secondary index. */
  SecondaryIndex *year_index; /**< The year secondary index. */
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
 * Performs insertions in the database
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
 * @brief Creates a new adapter structure.
 *
 * @return A new Adapter. 
 */
Adapter *adapter_new (void);

/**
 * Removes something from the database
 *
 * @param db Pointer to Adapter struct
 */
void adapter_remove (Adapter * db);

#endif
