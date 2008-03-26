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

int cmpstring(char **p1, char **p2){

  return strcmp((const char*) *p1, (const char*) *p2);
}

int makeArrayPKIndex(char **pkindex, FILE * base, int numreg){

  int i;

  if(!base)
    return 1;

  fseek(base, 0, SEEK_SET);

  for( i = 0; i<numreg; i++){
    readValue(pkindex[i], 200, base);
    fseek(base, 250, SEEK_CUR);
    sprintf(&pkindex[i][200], "%010d", i);
  }

  qsort(pkindex, numreg, sizeof(char*), (const void *)cmpstring);

  return 0;

}

int makeFilePKIndex(char ** pkindex, FILE * out, int numreg){
  
  int i;

  for(i = 0; i< numreg; i++){
    fprintf(out, "%-210s", pkindex[i]);
  }

  return 0;
}

int loadPkFile(char **pkindex, FILE * pkfile){

  if(!pkfile)
    return 1;
  
  int size, numpk,i;
  char file_array[210];

  size = getFileSize(pkfile);
  numpk = size/210;

/*   /\* Create the pk table in the memory*\/ */
/*   pkindex = (char**)malloc(sizeof(char*) * numpk); */
/*   for( i = 0; i < numpk; i++){ */
/*     pkindex[i] = (char*)malloc(sizeof(char) * 210); */
/*   } */

  /* Copy the primary keys of the pkfile and put them in the memory pk table*/

  for(i=0; i<numpk; i++){
    fgets(file_array, 211, pkfile);

    /*Essa porra aqui fica lendo lixo no file_array, se eh que le alguma coisa... */
  }
  
  for(i =0; i< numpk; i++){
    printf("%s\n", pkindex[i]);
  }

  return 0;
}

int main(){

  FILE * base;
  FILE * out;
  int i, size;
  int numreg;
  char **pkindex;
  char **pkteste;


  
  base = fopen("base01.dat", "r");
  out = fopen("out.dat", "w");

  size = getFileSize(base);
  
  numreg = size / REG_SIZE;


  /*Allocating some memory for our PK table.*/
  pkindex = (char**)malloc(sizeof(char*) * numreg);

  for( i = 0; i < numreg; i++){
    pkindex[i] = (char*)malloc(sizeof(char) * 210);
  }
  /**/



  makeArrayPKIndex(pkindex, base, numreg);



  makeFilePKIndex(pkindex, out, numreg);

  /*Printing for debugging... */
  for( i = 0; i < numreg; i++){
    printf("%s\n", pkindex[i]);
  }

  fclose(out);

  /*Agora vem a parte de ler o arquivo na memoria.*/

  out = fopen("out.dat", "r");

  pkteste = (char**)malloc(sizeof(char*) * numreg);

  for( i = 0; i < numreg; i++){
    pkteste[i] = (char*)malloc(sizeof(char) * 210);
  }

  loadPkFile(pkteste, out);

  return 0;
}
