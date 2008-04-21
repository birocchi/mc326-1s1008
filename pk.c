#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "pk.h"

/**
 * \brief   Comparision function used when performing a binary search.
 * \param   a The key being compared to the whole list.
 * \param   b A PrimaryKeyRecord which is being compared to \a a.
 * \return  Returns the value of \a strcmp on \a a and \a b.
 */
static int __bsearch_compare(const void* a, const void* b)
{
  return strcmp( (char*)a, ((PrimaryKeyRecord*)b)->name );
}

/**
 * \brief   Comparison function used when performing a quicksort.
 * \param   a A PrimaryKeyRecord.
 * \param   b Another PrimaryKeyRecord.
 * \return  Returns the value of \a strncmp on \a and \a b.
 */
static int __qsort_compare(const void* a, const void* b)
{
  return strncmp( ((PrimaryKeyRecord*)a)->name, ((PrimaryKeyRecord*)b)->name, TITLE_LENGTH );
}

/**
 * \brief Allocate more memory for the primary keys index.
 * \param index The primary index list which we want to expand.
 *
 * Allocate more memory for the primary keys index, doubling
 * the space used each time it is called. Aborts on error.
 */
static void pkListInflateSize(PrimaryKeyList* index) {
  PrimaryKeyRecord* tmp;

  tmp = realloc(index->pklist, (index->maxregs*2)*sizeof(PrimaryKeyRecord));

  assert(tmp != NULL);

  index->pklist = tmp;
  index->maxregs = index->maxregs*2;
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

  /* This will use bsearch to find the key. */
  match = bsearch(key, index->pklist, index->regnum,
                  sizeof(PrimaryKeyRecord), __bsearch_compare);

  if (!match)
    return -1;
  else
    return match->rrn;
}

int pkListInsert(PrimaryKeyList* index, const char* name) {
  if (pkListFindByName(index, name) != -1)
    return 1;

  /* If we need more space on the list, inflate it */
  if (index->regnum == index->maxregs)
    pkListInflateSize(index);

  /* New register's rrn is the number of registers,
   * since it's added to the end. */
  index->pklist[index->regnum].rrn = index->regnum;
  index->regnum++;

  strncpy(index->pklist[index->regnum].name, name, TITLE_LENGTH+1);

  qsort(index->pklist, index->regnum, sizeof(PrimaryKeyRecord),
        __qsort_compare);

  return 0;
}

int pkListIsEmpty(PrimaryKeyList* index) {
  if (index)
    return index->regnum == 0;

  return 1;
}

PrimaryKeyList* pkListLoad(const char* base_name, const char* pkname) {
  if (!fileExists(pkname))
    return pkListLoadFromBase(base_name);
  else {
    if (fileExists(base_name))
      return pkListLoadFromPK(pkname);
    else
      /* If neither the PK index nor the database exist,
       * return a new, empty list */
      return pkListNew(0);
  }
}

PrimaryKeyList* pkListLoadFromBase(const char* base_name) {
  PrimaryKeyList* index;
  FILE* base;
  int i;

  base = fopen(base_name, "r");
  assert(base != NULL);

  index = pkListNew(getFileSize(base) / REG_SIZE);

  /* Next we read the registers from our base. */
  for (i = 0; i < index->regnum; i++) {
    fgets(index->pklist[i].name, TITLE_LENGTH+1, base);
    index->pklist[i].rrn = i;

    /* Strip trailing whitespaces from the name */
    stripWhiteSpace(index->pklist[i].name);

    /* Skip the rest of the entry and go to the next record */
    fseek(base, REG_SIZE - TITLE_LENGTH, SEEK_CUR);
  }

  qsort(index->pklist, index->regnum, sizeof(PrimaryKeyRecord),
        __qsort_compare);

  fclose(base);

  return index;
}

PrimaryKeyList* pkListLoadFromPK(const char* pkname) {
  FILE* pkfile;
  PrimaryKeyList* index;
  char tmprrn[RRN_LENGTH+1];
  int i, rrn;

  pkfile = fopen(pkname, "r");
  assert(pkfile != NULL);

  index = pkListNew(getFileSize(pkfile) / PK_REG_SIZE);

  for (i = 0; i < index->regnum; i++) {
    fgets(index->pklist[i].name, TITLE_LENGTH+1, pkfile);

    /* Strip trailing whitespaces from the name */
    stripWhiteSpace(index->pklist[i].name);

    fgets(tmprrn, RRN_LENGTH+1, pkfile);
    rrn = atoi(tmprrn);
    index->pklist[i].rrn = rrn;
  }

  fclose(pkfile);

  return index;
}

PrimaryKeyList* pkListNew(size_t nelem) {
  PrimaryKeyList* ret;

  ret = MEM_ALLOC(PrimaryKeyList);
  ret->regnum  = nelem;

  /* If nelem == 0, we create an empty list
   * twice as big as the initial size of the base */
  ret->maxregs = (nelem > 0 ? 2*nelem : 40);

  ret->pklist  = MEM_ALLOC_N(PrimaryKeyRecord, ret->maxregs);

  return ret;
}

int pkListRemove(PrimaryKeyList* index, const char* name, int * rrn){
  PrimaryKeyRecord* match;
  int i = 0, j;

  /* This will use bsearch to find the key. */
  match = bsearch(name, index->pklist, index->regnum,
                  sizeof(PrimaryKeyRecord),
                  __bsearch_compare);

  /* If the name isn't there, we can't remove it. */
  if (!match)
    return 1;

  /* Return via pointer the match's rrn. */
  *rrn = match->rrn;

  /* Find the register in index->pklist. */
  while (index->pklist[i].rrn != match->rrn){
    i++;
  }
  /* Move all the subsequent register back one place. */
    for(j = i; j < index->regnum - 1; j++){
      index->pklist[j] = index->pklist[j+1];
    }

  index->regnum--;

  /* Must keep it sorted. */
  qsort(index->pklist, index->regnum,
	sizeof(PrimaryKeyRecord), __qsort_compare);

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
