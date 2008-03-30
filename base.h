#ifndef __BASE_H
#define __BASE_H

#include <stdio.h>

/* Defining the sizes of each field. */
#define TITLE_LENGTH   200
#define TYPE_LENGTH    100
#define AUTHOR_LENGTH  125
#define YEAR_LENGTH    4
#define VALUE_LENGTH   12
#define IMG_LENGTH     9

/* Total size of a register. */
#define REG_SIZE (TITLE_LENGTH + TYPE_LENGTH + AUTHOR_LENGTH + \
                  YEAR_LENGTH + VALUE_LENGTH + IMG_LENGTH)

/* The main structure which holds data about
 * each register in the database */
typedef struct
{
	char title[TITLE_LENGTH+1];       /* The artwork's title */
	char type[TYPE_LENGTH+1];         /* The artwork's type  */
	char author[AUTHOR_LENGTH+1];     /* The artworks author */
  char year[YEAR_LENGTH+1];         /* The year the artwork was made */
	char value[VALUE_LENGTH+1];       /* The artwork's value */
	char img[IMG_LENGTH+1];           /* The register's identifier */
} artwork_info;

/*
 * getValidImagePath
 *
 * Receives the image identifier and returns it with the '.' before
 * the extension.
 * One must save the returned pointer and free it after using it.
 */
char* baseGetValidImagePath(const char* s);

/*
 * baseIsValidIdentifier
 *
 * Checks if the image identifier is valid.
 * This time we use strtol to get past the digits and also
 * check the file extension.
 *
 * Returns 1 on error and 0 for OK.
 */
int baseIsValidIdentifier(const char* name);

/*
 * readArtworkRecord
 *
 * Reads one record from the database file /base/ and
 * stores it in /info/.
 *
 * Returns 0 on succes and 1 on error.
 */
int baseReadArtworkRecord(FILE* base, artwork_info* info);

/*
 * writeData
 *
 * Writes the data from the struct pointed at by *info
 * to the file pointed at by *file.
 * Writes it according to the requested parameters.
 */
int baseWriteData(FILE *file, artwork_info *info);

#endif
