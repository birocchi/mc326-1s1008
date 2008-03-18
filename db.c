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

int makeIndex(FILE * base, FILE * index, char * pkindex){
  
  int i = 0, size, numreg;
  char * pk;

  if(!base || !index){
    printf("Erro na geracao de indice de chaves primarias.\n");
    return 1;
  }

  size = getFileSize(base);
  
  rewind(base);

  numreg = size/REG_SIZE;

  pk = (char*)malloc(NAME);

  for(i = 0; i < numreg; i++){
    fseek(base, i*REG_SIZE , 0);
    fread(pk, 1, NAME, base);
    fprintf(index, "%-200s%05d", pk, i);
  }

  return 0;
}

int main(){

  FILE * base;
  FILE * out;
  char * pkindex;

  base = fopen("base01.dat", "r");
  out = fopen("out.dat", "a");

  makeIndex(base, out, pkindex);
  
  return 0;
}
