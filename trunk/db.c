#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "db.h"
#include "file.h"

/*
 * __bsort_compare
 *
 * Comparison function used when we call bsort to look for a key in our array.
 * It works like __qsort_compare, however /a/ is just a character here, but a
 * primary_key structure in __qsort_compare.
 */
static int __bsort_compare(const void* a, const void* b)
{
  return strncmp( (char*)a, ((primary_key*)b)->name, strlen(a));
}

/*
 * __qsort_compare
 *
 * Comparison function used when we call qsort in our primary keys array.
 * It just calls strncmp on primary_key's name member.
 */
static int __qsort_compare(const void* a, const void* b)
{
  return strncmp( ((primary_key*)a)->name, ((primary_key*)b)->name, NAME_LENGTH);
}

primary_key* createFirstPK(){

  primary_key * index;
  
  index = (primary_key*)calloc(20, sizeof(primary_key));

  return index;
}

primary_key* createPKFromBase(FILE* base, int* regcount) {
  int i, numreg;
  primary_key* index;

  if (base == NULL)
    return NULL;

  numreg = getFileSize(base) / REG_SIZE;
  *regcount = numreg;

  index = (primary_key*)calloc(numreg, sizeof(primary_key));
  if (index == NULL)
    return NULL;

  for (i = 0; i < numreg; i++) {
    fgets(index[i].name, NAME_LENGTH, base);
    index[i].rrn = i;
    fseek(base, (REG_SIZE - NAME_LENGTH)+1, SEEK_CUR);
  }

  qsort(index, numreg, sizeof(primary_key), __qsort_compare);

  fseek(base, 0, SEEK_SET);

  return index;
}

primary_key* incrementPK(primary_key* index, int regcount, artwork_info * info){

  if(index){
    
    if(regcount && !(regcount % 20)){
      index = realloc(index, (regcount + 20) * PK_REG_SIZE);
    }
    
    index[regcount].rrn = regcount;
    strncpy(index[regcount].name, info->title, NAME_LENGTH);
    qsort(index, regcount, sizeof(primary_key), __qsort_compare);
    return index;
  }
  
  return NULL;
}

int findEntry(primary_key* index, const char* key, int numreg) {
  primary_key* match;

  match = bsearch(key, index, numreg, sizeof(primary_key), __bsort_compare);
  if (match == NULL)
    return -1;
  else
    return match->rrn;
}

primary_key* loadPKFile(FILE* pkfile, int* regcount) {
  char tmpname[NAME_LENGTH+1], tmprrn[RRN_LENGTH+1];
  int i, numreg, rrn;
  primary_key* index;

  if (pkfile == NULL)
    return NULL;

  numreg = getFileSize(pkfile) / PK_REG_SIZE;
  *regcount = numreg;

  index = (primary_key*)calloc(numreg, sizeof(primary_key));
  if (index == NULL)
    return NULL;

  for (i = 0; i < numreg; i++) {
    fgets(tmpname, NAME_LENGTH+1, pkfile);
    fgets(tmprrn, RRN_LENGTH+1, pkfile);
    rrn = atoi(tmprrn);
    strncpy(index[i].name, tmpname, NAME_LENGTH);
    index[i].rrn = rrn;
  }

  return index;
}

void writePKToFile(primary_key* index, FILE* pkfile, int numreg) {
  char write_str[20] = {'\0'};
  int i;

  if (pkfile) {
    for (i = 0; i < numreg; i++) {
      sprintf(write_str, "%%-%ds%%0%dd", NAME_LENGTH, RRN_LENGTH);
      fprintf(pkfile, write_str, index[i].name, index[i].rrn);
    }
  }
} 

