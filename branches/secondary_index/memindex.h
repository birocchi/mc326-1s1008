#ifndef __MEMINDEX_H
#define __MEMINDEX_H

#include <stdlib.h>
#include "base.h"

/** Relative record number: 32-bit integer (4 bytes) */
#define RRN_LENGTH    4
#define MEM_REG_SIZE  (TITLE_LENGTH + RRN_LENGTH)

/**
 * Holds the PK info.
 */
typedef struct
{
  int rrn;
  char name[TITLE_LENGTH + 1];
} MemoryIndexRecord;

/**
 * Encapsulates all the info about the index.
 */
typedef struct
{
  char *fp_name;
  size_t regnum;
  size_t maxregs;
  MemoryIndexRecord *reclist;
} MemoryIndex;

/**
 * @brief Compares the two parameters
 * @param a Pointer to something.
 * @param b Pointer to something.
 * @return 0 or 1.
 */
int memory_index_compare_by_name (const void *a, const void *b);

/**
 * @brief Searches the string passed in the index.
 * @param index Pointer to a MemoryIndex struct.
 * @param name String to be searched.
 * @return A pointer to the record.
 */
MemoryIndexRecord *memory_index_find (MemoryIndex * index, const char *name);

/**
 * @brief Frees the allocated memory.
 * @param index Pointer to MemoryIndex 
 */
void memory_index_free (MemoryIndex * index);

/**
 * @brief Inserts string \a name in the position \a rrn in \a index
 * @param index Pointer to MemoryIndex struct.
 * @param name String.
 * @param rrn The relative register position.
 */
void memory_index_insert (MemoryIndex * index, const char *name, int rrn);

/**
 * @brief Checks whether the index is empty.
 * @param index Pointer to MemoryIndex struct.
 * @return 0 or 1.
 */
int memory_index_is_empty (MemoryIndex * index);

/**
 * @brief Loads all the data from the files in the disk.
 * @param index pointer to MemoryIndex struct.
 * @param filename String name of the file.
 */
void memory_index_load_from_file (MemoryIndex * index, const char *filename);

/**
 * @brief Creates all the data for the MemoryIndex struct.
 * @param fp_name String.
 * @param nelem The number of elements.
 * @return Pointer to MemoryIndex struct newly allocated.
 */
MemoryIndex *memory_index_new (const char *fp_name, size_t nelem);

/**
 * @brief Removes an specified entry from the indexes.
 * @param index Pointer to MemoryIndex struct.
 * @param rrn The relative register number.
 */
void memory_index_remove (MemoryIndex * index, int rrn);

#endif
