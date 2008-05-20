#ifndef __AVAIL_H
#define __AVAIL_H

#include <stdlib.h>

/**
 * The Available List Structure.
 */
typedef struct
{
  char *filename;
  size_t page_size;
  int tail;
} AvailList;

/**
 * @brief Frees memory allocated for an avail list.
 * @param avlist The list that should be freed.
 */
void avail_list_free (AvailList * avlist);

/**
 * @brief Returns the next available position
 *        in the avail list.
 * @param avlist The avail list being used.
 * @return The next available position in the
 *         avail list, or -1 if none is available.
 */
int avail_list_get_tail (AvailList * avlist);

/**
 * @brief Checks if the avail list is empty.
 * @param avlist The avail list being used.
 * @retval 0 The avail list is not empty.
 * @retval 1 The avail list is empty.
 */
int avail_list_is_empty (AvailList * avlist);

/**
 * @brief Loads the avail list's first position.
 * @param avlist The avail list being used.
 */
void avail_list_load (AvailList * avlist);

/**
 * @brief Create a new available list.
 * @param filename The name of the avail list in disk.
 * @param page_size The size of a record in the base which
 *                  uses the avail list.
 * @return A new \a AvailList.
 */
AvailList *avail_list_new (const char *filename, size_t page_size);

/**
 * @brief Gets the last item of the avail list.
 * @param avlist The avail list being used.
 * @param fp     The file related to the avail list.
 * @return The last available spot.
 */
int avail_list_pop (AvailList * avlist, FILE * fp);

/**
 * @brief Adds a new item to the avail list.
 *
 * @param avlist  The avail list being used.
 * @param fp      The file related to the avail list.
 * @param pos     The available position.
 */
void avail_list_push (AvailList * avlist, FILE * fp, int pos);

#endif
