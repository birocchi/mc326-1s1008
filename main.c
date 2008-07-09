#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "menu.h"

                     /*colocar o nome do nodo raiz aki*/
#define "BPNODE_NAME "0.bpnode"

int main() {
  BPNode *node;
  char option;
  int registro,busca;

  /*Primeiro tenta carregar o arquivo pra memoria*/
  if( !file_exists("BPNODE_NAME") ){
    node = /*funcao que carrega o arquivo e retorna um apontador pro nodo raíz*/
  
  printWelcome();

  do {
    printMainMenu ();

    switch (menuMultipleAnswers ("   Opcao desejada: ", "irbs"))) {
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
   
      default:
        break;
    }
  } while(option);
  
  /* Frees the memory used by the B+tree */
  free(node);

  return 0;
}
