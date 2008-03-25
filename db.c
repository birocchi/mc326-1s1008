#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include <string.h>


int getFileSize(FILE * file){

  if(!file){
    printf("Erro ao verificar tamanho do arquivo.\n");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  return ftell(file);

}

int makeFileIndex(FILE * base, FILE * index, char ** pkindex){
  
  int i = 0, size, numreg;
  char * pk;

  if(!base || !index){
    printf("Erro na geracao de indice de chaves primarias.\n");
    return 1;
  }

  size = getFileSize(base);
  
  rewind(base);

  numreg = size/REG_SIZE;

  pk = (char*)malloc(NAME_LENGTH);

  for(i = 0; i < numreg; i++){
    fseek(base, i*REG_SIZE , 0);
    fread(pk, 1, NAME_LENGTH, base);
    fprintf(index, "%-200s%05d", pk, i);
  }

  return 0;
}

int main(){

  FILE * base;
  FILE * out;
  char ** pkindex;
  int i, size;
  
  base = fopen("base01.dat", "r");
  out = fopen("out.dat", "a");

  size = getFileSize(base) / REG_SIZE;

  pkindex = (char**)malloc(sizeof(char**) * size);
  for(i = 0; i < size; i++){
    pkindex[i] = (char*)malloc(sizeof(char*NAME + 1));
  }

  

  makeFileIndex(base, out, pkindex);
  
  return 0;
}
