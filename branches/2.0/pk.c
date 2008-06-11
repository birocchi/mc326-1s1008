#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "pk.h"

/*
 * __bsearch_compare
 *
 * Comparison function used when we call bsearch to look for a key in our array.
 * It works like __qsort_compare, however /a/ is just a character here, but a
 * primary_key structure in __qsort_compare.
 */
static int __bsearch_compare(const void* a, const void* b)
{
  return strcmp( (char*)a, ((PrimaryKeyRecord*)b)->name );
}

/*
 * __qsort_compare
 *
 * Comparison function used when we call qsort in our primary keys array.
 * It just calls strncmp on primary_key's name member.
 */
static int __qsort_compare(const void* a, const void* b)
{
  return strncmp( ((PrimaryKeyRecord*)a)->name, ((PrimaryKeyRecord*)b)->name, TITLE_LENGTH );
}

/*
 * pkListInflateSize
 *
 * Function used internally. It allocates more memory for our primary keys
 * list, doubling the space used each time it is called.
 *
 * Returns 1 on error and 0 otherwise.
 */
static int pkListInflateSize(PrimaryKeyList* index) {
  PrimaryKeyRecord* tmp;

  tmp = realloc(index->pklist, (index->maxregs*2)*sizeof(PrimaryKeyRecord));

  if (tmp == NULL)
    return 1;
  else {
    index->pklist = tmp; 
    index->maxregs = index->maxregs*2;
  }

  return 0;
}

PrimaryKeyList* pkListInit(void) {
  PrimaryKeyList* ret;

  ret = MEM_ALLOC(PrimaryKeyList); /* Alocating the PK list struct. */
  ret->regnum = 0;
  ret->maxregs = 20; /* Initial size of the base */

  /* Alocate the actual PK list, for the first time. */
  ret->pklist = MEM_ALLOC_N(PrimaryKeyRecord, ret->maxregs); 

  if (ret->pklist == NULL) {
    free(ret);
    return NULL; 
  }

  return ret;
}

void pkListFree(PrimaryKeyList* index) {
  if (index) {
    /* PrimaryKeyList* points to a struct, 
     * so we must free the struct's pointers first.
     */
    free(index->pklist);
    free(index);
    index = NULL;
  }
}

int pkListFindByName(PrimaryKeyList* index, const char* key) {
  PrimaryKeyRecord* match;

  /* This will use bsearch to find the key. */
  match = bsearch(key, index->pklist, index->regnum,
                  sizeof(PrimaryKeyRecord),
                  __bsearch_compare);

  if (match == NULL)
    return -1;
  else
    return match->rrn;
}

int pkListInsert(PrimaryKeyList* index, const char* name) {
  /* If the name is already on the list, do not add it again */
  if (pkListFindByName(index, name) != -1)
    return 1;

  /* If we need more space on the list, inflate it */
  if (index->regnum == index->maxregs) {
    if (pkListInflateSize(index)) /* If we had problems inflating the index. */
      return 1;
  }

  /* New register's rrn is the number of registers, 
   * since it's added to the end. */
  index->pklist[index->regnum].rrn = index->regnum; 
  /* Then we copy the key, in this case the name, to the PK index. */
  strncpy(index->pklist[index->regnum].name, name, TITLE_LENGTH+1);

  index->regnum++;
  
  /* Must keep it sorted. */
  qsort(index->pklist, index->regnum, 
	      sizeof(PrimaryKeyRecord), __qsort_compare);

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

  /* While we have more registers to store than available space,
     we keep inflating our list.*/
  while (index->regnum > index->maxregs) {
    if (pkListInflateSize(index))
      return 1;
  }
  
  /* Next we read the registers from our base. */
  for (i = 0; i < index->regnum; i++) {
    fgets(index->pklist[i].name, TITLE_LENGTH, base);
    index->pklist[i].rrn = i;

    /* Strip trailing whitespaces from the name */
    stripWhiteSpace(index->pklist[i].name);

    /* Since we need to skip the rest of the register, and have already
     * read TITLE_LENGTH, we must go REG_SIZE - TITLE_LENGTH positions ahead,
     * and yet one more for we must read the next name. */
    fseek(base, (REG_SIZE - TITLE_LENGTH)+1, SEEK_CUR);
  }

  /* After everything is added, it has to be sorted. */
  qsort(index->pklist, index->regnum, sizeof(PrimaryKeyRecord), __qsort_compare);

  fseek(base, basepos, SEEK_SET);

  return 0;
}

int pkListLoadFromPK(PrimaryKeyList* index, FILE* pkfile) {
  char tmpname[TITLE_LENGTH+1], tmprrn[RRN_LENGTH+1];
  int i, rrn;
  int pkfilepos;

  if (pkfile == NULL)
    return 1;

  pkfilepos = ftell(pkfile);

  index->regnum = getFileSize(pkfile) / PK_REG_SIZE;

  /* While we have more registers to store than available space,
   * we keep inflating our list. */
  while (index->regnum > index->maxregs) {
    if (pkListInflateSize(index))
      return 1;
  }
  
  for (i = 0; i < index->regnum; i++) {
    fgets(tmpname, TITLE_LENGTH+1, pkfile);
    strncpy(index->pklist[i].name, tmpname, TITLE_LENGTH);

    /* Strip trailing whitespaces from the name */
    stripWhiteSpace(index->pklist[i].name);

    fgets(tmprrn, RRN_LENGTH+1, pkfile);
    rrn = atoi(tmprrn);
    index->pklist[i].rrn = rrn;
  }

  /* Change the pointer to it's original position. */
  fseek(pkfile, pkfilepos, SEEK_SET);

  return 0;
}

void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile) {
  char write_str[20] = {'\0'};
  int i;
  
  if (pkfile) {
    for (i = 0; i < index->regnum; i++) {
      /* We write the format string for fprintf at write_str.
	     * This takes the sizes for the fields. */
      sprintf(write_str, "%%-%ds%%0%dd", TITLE_LENGTH, RRN_LENGTH);
      fprintf(pkfile, write_str, index->pklist[i].name, index->pklist[i].rrn);
    }
  }
}