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
 * This structure encapuslates and holds
 * all the information related to a memory index.
 */
typedef struct
{
  char *fp_name; /**< The name of the disk representation of the index. */
  size_t regnum; /**< The current number of used entris. */
  size_t maxregs; /**< The total number of allocated entries. */
  MemoryIndexRecord *reclist; /**< The record list. */
} MemoryIndex;

/**
 * @brief Case-insensitive \a MemoryIndexRecord comparison.
 *
 * @param a Pointer to a \a MemoryIndexRecord.
 * @param b Pointer to another \a MemoryIndexRecord.
 *
 * @retval -1 a < b.
 * @retval 0  a == b.
 * @retval 1  a > b.
 */
int memory_index_compare_by_name (const void *a, const void *b);

/**
 * @brief Searches the memory index.
 *
 * @param index The memory index.
 * @param name  String to look for.
 *
 * @return A pointer to the record found,
 * or NULL if nothing is found.
 */
MemoryIndexRecord *memory_index_find (MemoryIndex * index, const char *name);

/**
 * @brief Frees the allocated memory for an index.
 *
 * @param index The memory index.
 */
void memory_index_free (MemoryIndex * index);

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
void memory_index_insert (MemoryIndex * index, const char *name, int rrn);

/**
 * @brief Checks whether the index is empty.
 *
 * @param index The index to check.
 *
 * @retval 0 The index is not empty.
 * @retval 1 The index is empty.
 */
int memory_index_is_empty (MemoryIndex * index);

/**
 * @brief Checks if the RRN exists in the index.
 *
 * @param index The memory index to check.
 * @param rrn   The RRN to look for.
 *
 * @retval 0 The RRN is not found.
 * @retval 1 The RRN is found.
 */
int memory_index_is_valid_rrn (MemoryIndex * index, int rrn);

/**
 * @brief Load data from disk.
 *
 * @param index    The memory index.
 * @param filename Name of the serialized index file.
 *
 * This function restores a previously saved memory index
 * into the current index.
 */
void memory_index_load_from_file (MemoryIndex * index, const char *filename);

/**
 * @brief Creates a new \a MemoryIndex.
 *
 * @param fp_name The name of the file to save the index in the end.
 * @param nelem   The initial number of elements.
 *
 * @return A new \a MemoryIndex.
 *
 * If \a nelem is 0, this function automatically allocates memory
 * for 40 entries.
 */
MemoryIndex *memory_index_new (const char *fp_name, size_t nelem);

/**
 * @brief Removes the specified entry from the index.
 *
 * @param index The memory index.
 * @param rrn   The relative register number.
 *
 * Please notice that as the index is sorted alphabetically,
 * this function takes O(n) to find the element and delete it.
 */
void memory_index_remove (MemoryIndex * index, int rrn);

#endif
