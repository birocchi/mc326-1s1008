#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "file.h"
#include "pk.h"

/*
 * __bsort_compare
 *
 * Comparison function used when we call bsort to look for a key in our array.
 * It works like __qsort_compare, however /a/ is just a character here, but a
 * primary_key structure in __qsort_compare.
 */
static int __bsort_compare(const void* a, const void* b)
{
  return strncmp( (char*)a, ((PrimaryKeyRecord*)b)->name, strlen(a));
}

/*
 * __qsort_compare
 *
 * Comparison function used when we call qsort in our primary keys array.
 * It just calls strncmp on primary_key's name member.
 */
static int __qsort_compare(const void* a, const void* b)
{
  return strncmp( ((PrimaryKeyRecord*)a)->name, ((PrimaryKeyRecord*)b)->name, NAME_LENGTH);
}

static int pkListInflateSize(PrimaryKeyList* index) {
  return 0;
}

PrimaryKeyList* pkListInit(void) {
  PrimaryKeyList* ret;

  ret = (PrimaryKeyList*)malloc(sizeof(PrimaryKeyList));
  if (ret == NULL)
    return NULL;

  ret->regnum = 0;
  ret->maxregs = 20; /* Initial size of the base */

  ret->pklist = (PrimaryKeyRecord*)calloc(ret->maxregs, sizeof(PrimaryKeyRecord));
  if (ret->pklist == NULL) {
    free(ret);
    return NULL;
  }

  return ret;
}

void pkListFree(PrimaryKeyList* index) {
  if (index) {
    free(index->pklist);
    free(index);
    index = NULL;
  }
}

int pkListFindByName(PrimaryKeyList* index, const char* key) {
  PrimaryKeyRecord* match;

  match = bsearch(key, index->pklist, index->regnum,
                  sizeof(PrimaryKeyRecord),
                  __bsort_compare);

  if (match == NULL)
    return -1;
  else
    return match->rrn;
}

int pkListInsert(PrimaryKeyList* index, int rrn, const char* name) {
  return 0;
}

int pkListIsEmpty(PrimaryKeyList* index) {
  if (index)
    return index->regnum == 0;
  else
    return 1;
}

int pkListLoadFromBase(PrimaryKeyList* index, FILE* base) {
  int basepos;
  int i;

  if ((base == NULL) || (index == NULL))
    return 1;

  basepos = ftell(base);
  index->regnum = getFileSize(base) / REG_SIZE;

  while (index->regnum > index->maxregs) {
    if (pkListInflateSize(index))
      return 1;
  }

  for (i = 0; i < index->regnum; i++) {
    fgets(index->pklist[i].name, NAME_LENGTH, base);
    index->pklist[i].rrn = i;
    fseek(base, (REG_SIZE - NAME_LENGTH)+1, SEEK_CUR);
  }

  qsort(index->pklist, index->regnum, sizeof(PrimaryKeyRecord), __qsort_compare);

  fseek(base, basepos, SEEK_SET);

  return 0;
}

int pkListLoadFromPK(PrimaryKeyList* index, FILE* pkfile) {
  char tmpname[NAME_LENGTH+1], tmprrn[RRN_LENGTH+1];
  int i, rrn;
  int pkfilepos;

  if (pkfile == NULL)
    return 1;

  pkfilepos = ftell(pkfile);
  index->regnum = getFileSize(pkfile) / PK_REG_SIZE;

  while (index->regnum > index->maxregs) {
    if (pkListInflateSize(index))
      return 1;
  }

  for (i = 0; i < index->regnum; i++) {
    fgets(tmpname, NAME_LENGTH+1, pkfile);
    fgets(tmprrn, RRN_LENGTH+1, pkfile);
    rrn = atoi(tmprrn);
    strncpy(index->pklist[i].name, tmpname, NAME_LENGTH);
    index->pklist[i].rrn = rrn;
  }

  fseek(pkfile, pkfilepos, SEEK_SET);

  return 0;
}

void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile) {
  char write_str[20] = {'\0'};
  int i;

  if (pkfile) {
    for (i = 0; i < index->regnum; i++) {
      sprintf(write_str, "%%-%ds%%0%dd", NAME_LENGTH, RRN_LENGTH);
      fprintf(pkfile, write_str, index->pklist[i].name, index->pklist[i].rrn);
    }
  }
}
