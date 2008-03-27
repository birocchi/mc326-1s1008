#ifndef __PK_H
#define __PK_H

#include "data.h"

#define PK_REG_SIZE (NAME_LENGTH + RRN_LENGTH)   /* 4 bytes for a 32-bit integer */

typedef struct {
  int rrn;
  char name[NAME_LENGTH+1];
} PrimaryKeyRecord;

typedef struct {
  int regnum;
  int maxregs;
  PrimaryKeyRecord* pklist;
} PrimaryKeyList;

PrimaryKeyList* pkListInit(void);
int pkListFindByName(PrimaryKeyList* index, const char* key);
void pkListFree(PrimaryKeyList* index);
int pkListInsert(PrimaryKeyList* index, int rrn, const char* name);
int pkListIsEmpty(PrimaryKeyList* index);
int pkListLoadFromBase(PrimaryKeyList* index, FILE* base);
int pkListLoadFromPK(PrimaryKeyList* index, FILE* pkfile);
void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile);

#endif
