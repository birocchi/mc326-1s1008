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

int makeArrayIndex(){


}


int main(){

  FILE * base;
  FILE * out;
  int i, size;
  
  base = fopen("base01.dat", "r");
  out = fopen("out.dat", "a");

  size = getFileSize(base) / REG_SIZE;

  return 0;
}
