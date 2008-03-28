#ifndef __PK_H
#define __PK_H

#include "data.h"

#define PK_REG_SIZE (NAME_LENGTH + RRN_LENGTH)   /* 4 bytes for a 32-bit integer */

/*
 * This is the internal structure used
 * by the primary keys list to store
 * each entry: it consists of a RRN and
 * one name.
 */
typedef struct {
  int rrn;
  char name[NAME_LENGTH+1];
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
 * list's internal structure.
 *
 * Returns a pointer to the initialized list or NULL
 * if it could not be initialized.
 */
PrimaryKeyList* pkListInit(void);

int pkListFindByName(PrimaryKeyList* index, const char* key);

void pkListFree(PrimaryKeyList* index);

int pkListInsert(PrimaryKeyList* index, const char* name);

int pkListIsEmpty(PrimaryKeyList* index);

int pkListLoadFromBase(PrimaryKeyList* index, FILE* base);

int pkListLoadFromPK(PrimaryKeyList* index, FILE* pkfile);

void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile);

#endif
