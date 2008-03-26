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
  return strncmp( (char*)a, ((primary_key*)b)->name, MIN(strlen(a), NAME_LENGTH));
}

primary_key* createPKFromBase(FILE* base, int* regcount) {
  int i, numreg;
  primary_key* index;

  if (base == NULL)
    return NULL;
  numreg = getFileSize(base) / REG_SIZE;

  index = (primary_key*)calloc(numreg, sizeof(primary_key));
  if (index == NULL)
    return NULL;

  for (i = 0; i < numreg; i++) {
    fgets(index[i].name, NAME_LENGTH, base);
    index[i].rrn = i;
    fseek(base, (REG_SIZE - NAME_LENGTH)+1, SEEK_CUR);
  }

  qsort(index, numreg, sizeof(primary_key), pk_cmpfunc);

  *regcount = numreg;

  return index;
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
  char tmpname[NAME_LENGTH], tmprrn[RRN_LENGTH];
  int i, numreg, rrn;
  primary_key* index;

  if (pkfile == NULL)
    return NULL;
  numreg = getFileSize(pkfile) / PK_REG_SIZE;

  index = (primary_key*)calloc(numreg, sizeof(primary_key));
  if (index == NULL)
    return NULL;

  for (i = 0; i < numreg; i++) {
    fgets(tmpname, NAME_LENGTH+1, pkfile);
    /*fgets(tmprrn, RRN_LENGTH+1, pkfile);
    rrn = atoi(tmprrn);*/
    fscanf(pkfile, "%04d", &rrn);
    strncpy(index[rrn].name, tmpname, NAME_LENGTH);
    index[rrn].rrn = rrn;
  }

  fseek(pkfile, 0, SEEK_SET);

  *regcount = numreg;

  return index;
}

void writePKToFile(primary_key* index, FILE* pkfile, int numreg) {
  char write_str[20] = {'\0'};
  int i;

  if (pkfile) {
    for (i = 0; i < numreg; i++) {
      sprintf(write_str, "%%-%ds%%0%dd", NAME_LENGTH, RRN_LENGTH);
      printf("%s %d\n", index[i].name, index[i].rrn);
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

  if(!f)
    return -1;

  fseek(f, 0, SEEK_END);
  file_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  return file_size;
}

#if 0
int cmpstring(char **p1, char **p2) {
  return strcmp((const char*) *p1, (const char*) *p2);
}

int makeArrayPKIndex(char **pkindex, FILE * base){

  int i, numreg;

  if(!base)
    return 1;

  numreg = getFileSize(base) / REG_SIZE;

  for( i = 0; i < numreg; i++){
    fgets(pkindex[i], NAME_LENGTH+1, base); /* NAME_LENGTH+1 because fgets reads n-1 characters */
    fseek(base, REG_SIZE-NAME_LENGTH, SEEK_CUR);
    sprintf(&pkindex[i][PK_NAME_LENGTH], "%010d", i);
  }

  qsort(pkindex, numreg, sizeof(char*), cmpstring);

  return 0;
}

int makeFilePKIndex(char ** pkindex, FILE * out, int numreg){
  
  int i;

  for(i = 0; i < numreg; i++){
    fprintf(out, "%-210s", pkindex[i]);
  }

  return 0;
}

int loadPkFile(char ** pkindex, FILE * pkfile){

  if(!pkfile)
    return 1;
  
  int size, numpk,i;

  size = getFileSize(pkfile);
  numpk = size/PK_SIZE;

  fseek(pkfile, 0, SEEK_SET);

  for(i = 0; i < numpk; i++) {
    fgets(pkindex[i], PK_SIZE+1, pkfile);
  }

  return 0;
}


char ** search(char **pkindex, char * key, int numreg){

  return bsearch(key, pkindex, numreg, sizeof(char*), (int)cmpstring);

}
#endif
