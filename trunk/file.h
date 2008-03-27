#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>

/*
 * fileExists
 *
 * Checks for the existence of a file with
 * name /filename/.
 *
 * Returns 1 if it exists, and 0 otherwise.
 */
int fileExists(const char* filename);

/*
 * getFileSize
 *
 * Returns the size in bytes of a file /f/
 * if /f/ is a valid file, and -1 otherwise.
 */
int getFileSize(FILE* f);

#endif
