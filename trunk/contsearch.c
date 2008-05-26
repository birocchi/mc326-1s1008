#include "contsearch.h"
#include <stdio.h>
#include <stdlib.h>

int numbits(char a){

  int n = 0;
  char comp = 1;

  while(comp) {
    if (a & comp)
      n++;
    comp <<= 1;
  }

  return n;

}

int main(){

  char a;


  a = 1; /* 00000001 */
  printf("%d\n", numbits(a));
  a = 0; /* 00000000 */
  printf("%d\n", numbits(a));
  a = 9; /* 00001001 */
  printf("%d\n", numbits(a));
  a = 255; /* 11111111 */
  printf("%d\n", numbits(a));
  

  while (1){
    scanf("%c", &a);
    printf("%d\n", numbits(a));
  }

  

  return 0;

}
