#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>

/**
 * Checks for the existence of a file with name filename.
 *
 * @param filename The name of the file we need to check.
 *
 * @retval 0 File does not exist.
 * @retval 1 File exists.
 */
int fileExists(const char* filename);

/**
 * Returns the size of a file.
 *
 * @param f The file whose size needs to be determined.
 *
 * @return The size in bytes of the file, -1 if f is an invalid file.
 */
int getFileSize(FILE* f);

#endif
