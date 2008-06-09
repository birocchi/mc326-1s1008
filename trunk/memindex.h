#ifndef __MEMINDEX_H
#define __MEMINDEX_H

#include <stdlib.h>
#include "base.h"

#define RRN_LENGTH 4 /**< Relative record number: 4-byte interger. */
#define MEM_REG_SIZE (TITLE_LENGTH + RRN_LENGTH)

/**
 * Structure which represents an entry
 * in the memory index.
 */
typedef struct
{
  int rrn; /**< The RRN of the entry */
  char name[TITLE_LENGTH + 1]; /**< The name of the entry */
} MemoryIndexRecord;

/**
 * This structure encapsulates and holds
 * all the information related to a memory index.
 */
typedef struct
{
  char *fp_name; /**< The name of the disk representation of the index. */

  size_t loaded_file; /**< The number of the currently loaded file (from the split list). */
  unsigned int (*hash_function) (char *); /**< The hash function to use with the keys. */

  size_t regnum; /**< The current number of used entries. */
  size_t maxregs; /**< The total number of allocated entries. */
  MemoryIndexRecord *reclist; /**< The record list. */
} MemoryIndex;

/**
 * @brief Searches the memory index.
 *
 * @param index The memory index.
 * @param name  String to look for.
 *
 * @return A pointer to the record found,
 * or NULL if nothing is found.
 */
MemoryIndexRecord *memory_index_find (MemoryIndex * index, char *name);

/**
 * @brief Frees the allocated memory for an index.
 *
 * @param index The memory index.
 */
void memory_index_free (MemoryIndex * index);

MemoryIndex * memory_index_get_next (MemoryIndex * index, char *key);

MemoryIndex * memory_index_get_previous (MemoryIndex * index, char *key);

/**
 * @brief Inserts an entry into the index.
 *
 * @param index The memory index.
 * @param name  The name of the new entry.
 * @param rrn   The relative register position.
 *
 * Please notice that this function does \b not check
 * if the name is already on the list, as this is a more
 * general-purpose index.
 * It is the responsibility of the programmer to check
 * if this is a duplicated entry.
 */
void memory_index_insert (MemoryIndex * index, char *name, int rrn);

/**
 * @brief Creates a new \a MemoryIndex.
 *
 * @param fp_name The name of the file to save the index in the end.
 *
 * @return A new \a MemoryIndex.
 *
 * This is a wrapper around \a memory_index_new_with_hash, using
 * \a hash_function as the hash function.
 */
MemoryIndex *memory_index_new (const char *fp_name);

/**
 * @brief Creates a new \a MemoryIndex.
 *
 * @param fp_name         The name of the file to save the index in the end.
 * @param hash_function   The hash function to be used.
 */
MemoryIndex *memory_index_new_with_hash (const char *fp_name,
                                         unsigned int (*hash_func) (char *));

/**
 * @brief Removes the specified entry from the index.
 *
 * @param index The memory index.
 * @param name  The name of the register to delete.
 */
void memory_index_remove (MemoryIndex * index, char * name);

#endif
