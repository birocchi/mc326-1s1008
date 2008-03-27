#ifndef __DB_H
#define __DB_H

#include <stdio.h>
#include "data.h"

#define PK_REG_SIZE (NAME_LENGTH + RRN_LENGTH)   /* 4 bytes for a 32-bit integer */

/*
 * Primary key structure. It is composed of a relative register
 * number and its associated name.
 */
typedef struct {
  int rrn;
  char name[NAME_LENGTH+1];
} primary_key;

primary_key* createFirstPK();

/*
 * createPKFromBase
 *
 * Generates the pk table from the database if there is
 * no primary keys file and store the number of entries
 * read in /regcount/.
 */
primary_key* createPKFromBase(FILE* base, int* regcount);

/*
 * findEntry
 *
 * Searches for key in the pk table.
 *
 * Returns the key's relative register number if it is found
 * in the table, and -1 otherwise.
 */
int findEntry(primary_key* index, const char* key, int numreg);

/*
 * loadPKFile
 *
 * Load the primary keys table from the pk file and store
 * the number of entries read in /regcount/.
 */
primary_key* loadPKFile(FILE* pkfilename, int* regcount);

/*
 * writePKToFile
 *
 * Writes the PK table to a file, basically.
 */
void writePKToFile(primary_key* index, FILE* pkfile, int numreg);

#endif
