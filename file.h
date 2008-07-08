#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>

/**
 * \brief Checks for the existence of a file with name filename.
 * \param filename The name of the file we need to check.
 * \retval 0 File does not exist.
 * \retval 1 File exists.
 */
int file_exists (const char *filename);

/**
 * \brief Returns the size of a file.
 * \param f The file whose size will be given.
 * \return The size in bytes of the file, -1 if f is an invalid file.
 */
long file_get_size (FILE * f);

/**
 * \brief Returns the size of a file with name \a filename.
 * \param filename The name of the file to be inspected.
 * \return The size in bytes of the file, -1 if f is an invalid file.
 *
 * This function works the same as \a file_get_size, but receives
 * a const char instead of a FILE as the argument.
 */
long file_get_size_from_name (const char *filename);

/**
 * @brief Checks if the file exists and is non-empty.
 *
 * @param filename The file name to check.
 *
 * @return 0 or 1 to show if it's valid.
 */
int file_is_valid (const char *filename);

#endif
