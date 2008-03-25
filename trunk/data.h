#ifndef __DATA_H_
#define __DATA_H_

#include <stdio.h>

/* Defining the sizes of each field. */
#define NAME_LENGTH 200
#define TYPE_LENGTH 100
#define AUTHOR_LENGTH 125
#define YEAR_LENGTH 4
#define VALUE_LENGTH 12
#define IMG_LENGTH 9
#define RRN 10 /* Relative record number */

/* Total size of a register. */
#define REG_SIZE (NAME_LENGTH + TYPE_LENGTH + AUTHOR_LENGTH + \
                  YEAR_LENGTH + VALUE_LENGTH + IMG_LENGTH)


/* Total size of a primary key. */
#define PK_SIZE (NAME_LENGTH + RRN)

/* The main structure which holds data about
 * each register in the database */
typedef struct
{
	char title[NAME_LENGTH];        /* The artwork's title */
	char type[TYPE_LENGTH];         /* The artwork's type  */
	char author[AUTHOR_LENGTH];     /* The artworks author */
	int year;                       /* The year the artwork was made */
	int value;                      /* The artwork's value */
	char img[IMG_LENGTH];           /* The register's identifier */
} artwork_info;

/*
 * writeData
 *
 * Writes the data from the struct pointed at by *info
 * to the file pointed at by *file.
 * Writes it according to the requested parameters.
 */
int writeData(FILE *file, artwork_info *info);

/*
 * validateIdentifier
 *
 * Checks if the image identifier is valid.
 * This time we use strtol to get past the digits and also
 * check the file extension.
 *
 * Returns 1 on error and 0 for OK.
 */
int validateIdentifier(const char* name);

#endif
