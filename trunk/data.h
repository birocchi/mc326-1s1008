#ifndef __DATA_H_
#define __DATA_H_

/* Defining the sizes of each field. */
#define NAME_LENGTH 200
#define TYPE_LENGTH 100
#define AUTHOR_LENGTH 125
#define YEAR_LENGTH 4
#define VALUE_LENGTH 12
#define IMG_LENGTH 9

/* Total size of a register. */
#define REG_SIZE (NAME_LENGTH + TYPE_LENGTH + AUTHOR_LENGTH + \
                  YEAR_LENGTH + VALUE_LENGTH + IMG_LENGTH)

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


#endif
