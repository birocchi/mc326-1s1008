#ifndef __SECINDEX_H
#define __SECINDEX_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "avail.h"
#include "memindex.h"

/**
 * The secondary index structure.
 */
typedef struct
{
  AvailList *avlist;        /**< The avail list. */
  FILE *fp_list;            /**< The entry-based list of titles and RRNs. */
  MemoryIndex *record_list; /**< The memory list of secondary indexes. */
} SecondaryIndex;

/**
 * @brief Apply a function to each entry related to a record.
 *
 * @param index The secondary index.
 * @param record The record to be iterated over.
 * @param callback The function which will be applied.
 * @param ... Additional parameters for \a callback.
 *
 * This function calls \a callback on each entry from the index file
 * related to an entry in the secondary index memory list.
 */
void secondary_index_foreach (SecondaryIndex * index,
                              MemoryIndexRecord * record,
                              void (*callback) (const char *, int, va_list),
                              ...);

/**
 * @brief Free memory allocated for a secondary index.
 *
 * @param index The secondary index.
 */
void secondary_index_free (SecondaryIndex * index);

/**
 * @brief Insert a new value into the secondary index.
 *
 * @param si_index The secondary index.
 * @param si_value The name of the index.
 * @param pk_value The related primary key value (title).
 *
 * This function inserts a value into the index, using avail lists
 * if necessary. If there is already a record with \a si_value as its
 * name, append the entry to the entry list file.
 */
void secondary_index_insert (SecondaryIndex * si_index, const char *si_value,
                             const char *pk_value);

/**
 * @brief Creates a new secondary index.
 *
 * @param indexname The file name of the serialized memory index part
 *                  of the secondary index.
 * @param listname  The name of the entry list file.
 * @param avname    The name of the avail list file.
 * @param writeonly Whether or not to load data from existing files.
 *
 * @return A new \a SecondaryIndex object.
 */
SecondaryIndex *secondary_index_new (const char *indexname,
                                     const char *listname, const char *avname,
                                     int writeonly);

/**
 * @brief Removes an entry from a secondary index.
 *
 * @param index The secondary index.
 * @param sec_value The value in the memory index part.
 * @param pk_value  The value of the primary key in the entry list file.
 */
void secondary_index_remove (SecondaryIndex * index, const char *sec_value,
                             const char *pk_value);

#endif