#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "data.h"
#include "db.h"

#define MIN(a,b) (a<b?a:b)

int pk_cmpfunc(const void* a, const void* b)
{
  return strncmp( ((primary_key*)a)->name, ((primary_key*)b)->name, NAME_LENGTH);
}

int pk_cmpfunc2(const void* a, const void* b)
{
  return strncmp( (char*)a, ((primary_key*)b)->name, strlen(a));
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

  qsort(index, numreg, sizeof(primary_key), pk_cmpfunc);

  return index;
}

primary_key* incrementPK(primary_key* index, int regcount, artwork_info * info){

  if(index){
    
    if(regcount && !(regcount % 20)){
      index = realloc(index, (regcount + 20) * PK_REG_SIZE);
    }
    
    index[regcount].rrn = regcount;
    strncpy(index[regcount].name, info->title, NAME_LENGTH);
    qsort(index, regcount, sizeof(primary_key), pk_cmpfunc);
    return index;
  }
  
  return NULL;
}

int findEntry(primary_key* index, const char* key, int numreg) {
  primary_key* match;

  match = bsearch(key, index, numreg, sizeof(primary_key), pk_cmpfunc2);
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
  printf("--> %d %d\n", numreg, *regcount);

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

int fileExists(const char* filename) {
  struct stat buf;

  if (stat(filename, &buf) != 0)
    return 0;

  if (!S_ISREG(buf.st_mode))
    return 0;

  return 1;
}

int getFileSize(FILE * f){
  int file_size;
  int prev_pos;

  if(!f)
    return -1;

  prev_pos = ftell(f);

  fseek(f, 0, SEEK_END);
  file_size = ftell(f);

  fseek(f, prev_pos, SEEK_SET);

  return file_size;
}
