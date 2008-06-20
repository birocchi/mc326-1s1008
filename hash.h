#ifndef __HASH_H
#define __HASH_H

/**
 * @brief The default hash function (Fowler/Noll/Vo algorithm)
 *
 * @param key The key to be hashed.
 *
 * @return The result of the hash function modulo \a INDEX_HASH_NUM. 
 */
unsigned int fnv1_hash (char *key);

/**
 * @brief Returns a name with the ".hXXXX" suffix appended to it.
 *
 * @param prefix  The base filename.
 * @param hashnum The number to append to the filename.
 * @param maxhash The maximum acceptable value for \a hashum.
 *
 * @return The filename with the suffix appended to it.
 *
 * Remember to free() the name returned by this function.
 */
char *hash_get_filename (const char *prefix, unsigned int hashnum,
                         unsigned int maxhash);

#endif
