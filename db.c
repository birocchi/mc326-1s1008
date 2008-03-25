#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include <string.h>


void readValue(char s[], size_t length, FILE * base)
{
  fgets(s, length+1, base);

}

int getFileSize(FILE * file){

  if(!file){
    printf("Erro ao verificar tamanho do arquivo.\n");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  return ftell(file);

}

int makeArrayIndex(char **pkindex, FILE * base, int numreg){

  int i;

  if(!base)
    return 1;

  fseek(base, 0, SEEK_SET);

  for( i = 0; i<numreg; i++){
    readValue(pkindex[i], 200, base);
    fseek(base, 250, SEEK_CUR);
    sprintf(&pkindex[i][200], "%010d", i);
  }

  return 0;

}

int main(){

  FILE * base;
  FILE * out;
  int i, size;
  int numreg;
  char **pkindex;


  
  base = fopen("base01.dat", "r");
  out = fopen("out.dat", "a");

  size = getFileSize(base);
  
  numreg = size / REG_SIZE;

  /*aki birocchi*/  
  pkindex = (char**)malloc(sizeof(char*) * numreg);

  for( i = 0; i < numreg; i++){
    pkindex[i] = (char*)malloc(sizeof(char) * 210);
  }

  /*ateh aki*/

  makeArrayIndex(pkindex, base, numreg);

  for( i = 0; i < numreg; i++){
    printf("%s\n", pkindex[i]);
  }

  return 0;
}
