#ifndef __HASH_H
#define __HASH_H

#define HASH_FILE_NUM 5 /**< Number of files the memory index is split into */

/**
 * @brief The default hash function (Fowler/Noll/Vo algorithm)
 *
 * @param key The key to be hashed.
 *
 * @return The result of the hash function modulo \a HASH_FILE_NUM.
 */
unsigned int hash_function (char *key);

/**
 * @brief Returns a name with the ".hXXXX" suffix appended to it.
 *
 * @param prefix  The base filename.
 * @param hashnum The number to append to the filename.
 *
 * @return The filename with the suffix appended to it.
 *
 * Remember to free() the name returned by this function.
 */
char *hash_get_filename (const char *prefix, unsigned int hashnum);

#endif