#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>

/**
 * @brief Checks for the existence of all files whose name have
 *        the same prefix and end in a value between 0 and \a maxhash
 *        and create any of them if necessary.
 *
 * @param prefix  The prefix shared by all files to be looked for.
 * @param maxhash The upper interval of the suffix numbers of the files.
 *
 * @retval 0 All files were present, none were created.
 * @retval 1 Some file has been created by the function.
 */
int file_create_if_needed (const char *prefix, size_t maxhash);

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

/**
 * @brief Checks if the file is valid and has a valid
 *        image extension (gif, jpg or png).
 *
 * @param filename The file name to check.
 *
 * @retval 0 The file or the file name are not valid.
 * @retval 1 The file and the file name are valid.
 */
int file_is_valid_image (const char *filename);

#endif
