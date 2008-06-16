#ifndef __DESCRIPTOR_H
#define __DESCRIPTOR_H

#include <stdlib.h>
#include "base.h"
#include "memindex.h"

#define DESC_HASH_NUM 9

/**
 * Similarity list entry. Contains the artwork
 * and its similarity to a given image.
 */
typedef struct
{
  double similarity;
  int rrn;
} SimilarityRecord;

/**
 * Similarity list. This structure is very similar to
 * \a MemoryIndex - it just contains the list and enough
 * information to grow it if needed.
 */
typedef struct
{
  size_t maxregs;
  size_t regnum;
  SimilarityRecord *list;
} SimilarityList;

/**
 * A descriptor structure. Contains the currently loaded file
 * and enough information to load another hash file.
 */
typedef struct
{
  char *fp_name;
  FILE *fp;
  unsigned int loaded_file;
} Descriptor;

/**
 * @brief Looks in the database for images similar to the given one.
 *
 * @param desc        The descriptor in use.
 * @param base        The base being used.
 * @param pk          The primary key list.
 * @param imgname     The filename of the base image.
 */
SimilarityList * descriptor_find (Descriptor *desc, Base *base, MemoryIndex *pk, char *imgname);

/**
 * @brief Frees memory allocated for a descriptor.
 *
 * @param desc The descriptor to free.
 */
void descriptor_free (Descriptor * desc);

/**
 * @brief Inserts an entry to the descriptor list.
 *
 * @param desc    The descriptor being used.
 * @param pkname  The primary index name of the entry.
 * @param d       The image's descriptor.
 *
 * Appends the entry to the correct descriptor file.
 */
void descriptor_insert (Descriptor * desc, const char *pkname, unsigned char d);

/**
 * @brief Creates a new structure.
 *
 * @param fp_name The prefix (common name) for all hash files.
 */
Descriptor *descriptor_new (const char *fp_name);

/**
 * @brief Frees memory allocated for a similarity list.
 *
 * @param simlist The similarity list being used.
 */
void simlist_free (SimilarityList * simlist);

/**
 * @brief Creates a new similarity list structure.
 *
 * @return A new \a SimilarityList structure.
 */
SimilarityList *simlist_new (void);

#endif
