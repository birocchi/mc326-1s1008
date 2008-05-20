#ifndef __PK_H
#define __PK_H

#include "base.h"

#define RRN_LENGTH  4                             /* Relative record number. 32-bit integer (4 bytes) */
#define PK_REG_SIZE (TITLE_LENGTH + RRN_LENGTH)   /* 4 bytes for a 32-bit integer */

/*
 * This is the internal structure used
 * by the primary keys list to store
 * each entry: it consists of a RRN and
 * one name.
 */
typedef struct {
  int rrn;
  char name[TITLE_LENGTH+1]; 
} PrimaryKeyRecord;

/*
 * This is the structure used everywhere
 * to handle primary keys.
 * There's the current number of entries,
 * the maximum number of entries for which
 * there is memory allocated, and the list
 * itself.
 */
typedef struct {
  int regnum;
  int maxregs;
  PrimaryKeyRecord* pklist;
} PrimaryKeyList;

/*
 * pkListInit
 *
 * The list's "constructor": allocate memory for the
 * list's internal structure, starting with 20 positions.
 *
 * Returns a pointer to the initialized list or NULL
 * if it could not be initialized.
 */
PrimaryKeyList* pkListInit(void);


/*
 * pkListFindByName
 *
 * Searches for the key 'key' in the 'index' list.
 *
 * Returns the key's rrn if found or -1 case not.
 */
int pkListFindByName(PrimaryKeyList* index, const char* key);


/*
 * pkListFree
 *
 * Entirely frees the struct pointed by 'index'. 
 * Frees the pointer to the list,
 * and only then frees the struct itselft.
 */
void pkListFree(PrimaryKeyList* index);

/*
 * pkListInsert
 *
 * Takes the pointer to the PrimaryKeyList,
 * and a string, the pk. Adds the new key
 * to the PK index.
 *
 * Returns 0 if everything went ok 
 * or 1 if the key was already in the list
 * or if it had any problems inflating the table
 * case it was already full.
 */
int pkListInsert(PrimaryKeyList* index, const char* name);

/*
 * pkListIsEmpty
 *
 * Checks if the PK table is empty.
 *
 * Returns 1 if the list is empty or 'index' is NULL, 0 otherwise.
 */
int pkListIsEmpty(PrimaryKeyList* index);

/*
 * pkListLoadFromBase
 *
 * Loads the primary keys from the registers at 'base'
 * to 'index'. Leaves it sorted.
 *
 * Returns 1 if any problems ocurred, 0 otherwise.
 */
int pkListLoadFromBase(PrimaryKeyList* index, FILE* base);

/*
 * pkListLoadFromPK
 *
 * Loads the primary keys from the primary key index
 * file 'pkfile' into 'index'.
 *
 * Returns 1 if any problems ocurres, 0 otherwise.
 */
int pkListLoadFromPK(PrimaryKeyList* index, FILE* pkfile);

/*
 * pkListWriteToFile
 *
 * Takes the primary key index 'index' and writes it properly
 * to the file 'pkfile'.
 */
void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile);

#endif
