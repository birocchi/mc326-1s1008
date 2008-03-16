#ifndef __DATA_H_
#define __DATA_H_

/* Defining the sizes of each field. */
#define NAME 200
#define TYPE 100
#define AUTHOR 125
#define YEAR 4
#define VALUE 12
#define IMG 9


typedef struct
{
	char title[NAME];
	char type[TYPE];
	char author[AUTHOR];
	int year;
	int value;
	char img[IMG];
} artwork_info;

#endif
