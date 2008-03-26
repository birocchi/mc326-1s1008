#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include <string.h>

int getFileSize(FILE * file){
  int file_size;

  if(!file) {
    fprintf(stderr, "getFileSize: Invalid file.\n");
    return -1;
  }

  file_size = fseek(file, 0, SEEK_END);
  fseek(file, 0, SEEK_SET);

  return file_size;
}

int cmpstring(char **p1, char **p2) {
  return strcmp((const char*) *p1, (const char*) *p2);
}

int makeArrayPKIndex(char **pkindex, FILE * base){

  int i, numreg;

  if(!base)
    return 1;

  numreg = getFileSize(base) / REG_SIZE;

  fseek(base, 0, SEEK_SET);

  for( i = 0; i < numreg; i++){
    fgets(pkindex[i], NAME_LENGTH+1, base);
    fseek(base, 250, SEEK_CUR);
    sprintf(&pkindex[i][200], "%010d", i);
  }

  qsort(pkindex, numreg, sizeof(char*), (const void *)cmpstring);

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

  for(i = 0; i < numpk; i++){
    fgets(pkindex[i], PK_SIZE+1, pkfile);
  }

  return 0;
}


char ** search(char **pkindex, char * key, int numreg){

  return bsearch(key, pkindex, numreg, sizeof(char*), (int)cmpstring);

}
