#include <stdio.h>
#include <stdlib.h>
#include "bplustree.h"

int main() {
  B+Tree tree;
  char option;
  int registro,busca;

  tree = getNewB+Tree(); 

  /*void loadBase(); alguma função que carregue os dados que ja estao na base*/


  do {
    printf("  #############\n");
    printf("  # (I)nserir #\n");
    printf("  # (R)emover #\n");
    printf("  # (B)uscar  #\n");
    printf("  # (S)air    #\n");
    printf("  #############\n");
    printf("  Escolha uma opção: ");
    scanf("%c", &option);

    switch( tolower(option) ) {
      case 'i':
        printf("Digite o registro a ser inserido: ");
        scanf("%d", &registro);
        /*void insert(); alguma função que insere*/
        break;

      case 'r':
        printf("Digite o registro a ser removido: ");
        scanf("%d", &registro);
        /*void remove(); alguma função que remove*/
        break;

      case 'b': 
        printf("Digite a o numero a ser buscado: ");
        scanf("%d", &busca);
        /*void search(); alguma função de busca*/
        break;

      case 's':
        break;
    }
  } while(option);
  
  /* Frees the memory used by the B+tree */
  free(tree);

  return 0;
}
