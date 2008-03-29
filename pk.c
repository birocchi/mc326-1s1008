#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "file.h"
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

static int pkListInflateSize(PrimaryKeyList* index) {
  PrimaryKeyRecord* tmp;

  /* We realloc the array to twice it's previous size. */
  tmp = realloc(index->pklist, (index->maxregs*2)*sizeof(PrimaryKeyRecord));

  if (tmp == NULL) /* If we had a problem reallocating it */
    return 1; /* Return an error with nothing changed to 'index'. */
  else { /* In the other hand, if it worked right */
    index->pklist = tmp; /* Now index may receive the bigger array. */
    index->maxregs = index->maxregs * 2; /* Now it has twice the storage space. */
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

  if (ret->pklist == NULL) { /* Checking the PK list alocation. */
    free(ret); /* If not, then the struct is useless, free it. */
    return NULL; 
  }

  return ret;
}

void pkListFree(PrimaryKeyList* index) {
  /* PrimaryKeyList* points to a struct, 
     so we must free the struct's pointers first*/
  
  if (index) {
    free(index->pklist); /* Free the pointer to the actual list. */
    free(index); /* And then free the struct. */
    index = NULL;
  }
}

int pkListFindByName(PrimaryKeyList* index, const char* key) {
  PrimaryKeyRecord* match;

  /* This will use bsearch to find the key. */
  match = bsearch(key, index->pklist, index->regnum,
                  sizeof(PrimaryKeyRecord),
                  __bsearch_compare);

  /* bsearch returns NULL if it doesn't find it.
     So we catch that and return -1. */
  if (match == NULL)
    return -1;
  else /* In case it's a valid pointer...*/
    return match->rrn; /* Return it's rrn. */
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
     since it's added to the end. */
  index->pklist[index->regnum].rrn = index->regnum; 
  /* Then we copy the key, in this case the name, to the PK index. */
  strncpy(index->pklist[index->regnum].name, name, TITLE_LENGTH);

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

  basepos = ftell(base); /* Saves the current position at the base. */
  index->regnum = getFileSize(base) / REG_SIZE; /* How many registers we have. */

  /* While we have more registers to store than available space,
     we keep inflating our list.*/
  while (index->regnum > index->maxregs) {
    if (pkListInflateSize(index))
      return 1;
  }
  
  /* Next we read the registers from our base. */
  for (i = 0; i < index->regnum; i++) {
    fgets(index->pklist[i].name, TITLE_LENGTH, base); /* Read the name. */
    index->pklist[i].rrn = i; /* Set it's rrn to the current position. */

    /* Since we need to skip the rest of the register, and have already
       read TITLE_LENGTH, we must go REG_SIZE - TITLE_LENGTH positions ahead,
       and yet one more for we must read the next name. */
    fseek(base, (REG_SIZE - TITLE_LENGTH)+1, SEEK_CUR);
  }

  /* After everything is added, it has to be sorted. */
  qsort(index->pklist, index->regnum, sizeof(PrimaryKeyRecord), __qsort_compare);

  /* Leave the pointer on base at it's original position. */
  fseek(base, basepos, SEEK_SET);

  return 0;
}

int pkListLoadFromPK(PrimaryKeyList* index, FILE* pkfile) {
  char tmpname[TITLE_LENGTH+1], tmprrn[RRN_LENGTH+1];
  int i, rrn;
  int pkfilepos;

  if (pkfile == NULL)
    return 1;

  /* Save the pointer's positions. */
  pkfilepos = ftell(pkfile);

  /* How many registers we have. */
  index->regnum = getFileSize(pkfile) / PK_REG_SIZE;

  /* While we have more registers to store than available space,
     we keep inflating our list.*/
  while (index->regnum > index->maxregs) {
    if (pkListInflateSize(index))
      return 1;
  }
  
  /* For each register in the pkfile... */
  for (i = 0; i < index->regnum; i++) {
    fgets(tmpname, TITLE_LENGTH+1, pkfile); /* Get the name to a temp. */
    fgets(tmprrn, RRN_LENGTH+1, pkfile); /* And so do it with the pk. */
    rrn = atoi(tmprrn); /* We change the string into a int. */
    /* Put the temporary into the definite position. */
    strncpy(index->pklist[i].name, tmpname, TITLE_LENGTH); 
    /* And save the rrn to the list. */
    index->pklist[i].rrn = rrn;
  }

  /* Change the pointer to it's original position. */
  fseek(pkfile, pkfilepos, SEEK_SET);

  return 0;
}

void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile) {
  char write_str[20] = {'\0'};
  int i;
  
  if (pkfile) { /* Check if the file is properly opened. */
    for (i = 0; i < index->regnum; i++) {
      /* We write the format string for fprintf at write_str.
	 This takes the sizes for the fields. */
      sprintf(write_str, "%%-%ds%%0%dd", TITLE_LENGTH, RRN_LENGTH);
      /* Using the format string, we call fprintf to write the
	 name and rrn fields to the pkfile. */
      fprintf(pkfile, write_str, index->pklist[i].name, index->pklist[i].rrn);
    }
  }
}
