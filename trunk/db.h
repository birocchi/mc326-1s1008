#ifndef __DB_H
#define __DB_H

#include <stdio.h>
#include "data.h"

#define PK_REG_SIZE (NAME_LENGTH + RRN_LENGTH)   /* 4 bytes for a 32-bit integer */

typedef struct {
  int rrn;
  char name[NAME_LENGTH+1];
} primary_key;


primary_key* createFirstPK();

/*
 * pk_cmpfunc
 * Comparison function for the primary keys, used by bsearch.
 */
int pk_cmpfunc(const void* a, const void* b);

/*
 * createPKFromBase
 * Generates the pk table from the database.
 * Used if there is no PK file.
 */
primary_key* createPKFromBase(FILE* base, int* regcount);

/*
 * findEntry
 * Searches for key in the pk table.
 */
int findEntry(primary_key* index, const char* key, int numreg);

/*
 * loadPKFile
 * Makes the table os primary keys from the pk file.
 */
primary_key* loadPKFile(FILE* pkfile, int* regcount);

/*
 * writePKToFile
 * Writes the PK table to a file, basically.
 */
void writePKToFile(primary_key* index, FILE* pkfile, int numreg);

/*
 * fileExists
 * Tests if the file actually exists.
 */
int fileExists(const char* filename);

/*
 * getFileSize
 * Returns the size, in bytes, of the file.
 */
int getFileSize(FILE * file);

/*
 * cmpstring
 * Used by qsort for string comparisons.
 */
int cmpstring(char **p1, char **p2);

/*
 * makeArrayPKIndex
 * Generates a table of primary keys from the database.
 */
int makeArrayPKIndex(char **pkindex, FILE * base);

/*
 * makeFilePKIndex
 * Saves into a 'out' the primary keys
 * table from 'pkindex'.
 */
int makeFilePKIndex(char ** pkindex, FILE * out, int numreg);

/*
 * loadPkFile
 * Loads the primary keys table from a file into
 * the RAM memory.
 */
int loadPkFile(char ** pkindex, FILE * pkfile);

/*
 * search
 * Performs a binary search and returns a pointer
 * to where it found the key.
 */
char ** search(char **pkindex, char * key, int numreg);

primary_key* incrementPK(primary_key* index, int regcount, artwork_info * info);


#endif
