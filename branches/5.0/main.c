#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "menu.h"

#define UINTLEN 10 /* In a 32-bit system, UINT_MAX has 10 digits */

int
main (int argc, char *argv[])
{
  BPTree *tree;
  BPNode *node;
  char strkey[UINTLEN+1], strvalue[UINTLEN+1];
  int key, value;
  int registro, busca;
  int halt = 0;

  tree = bptree_new ();
  
  printWelcome();

  do {
    printMainMenu ();

    switch (menuMultipleAnswers ("   Opcao desejada: ", "irbs")) {
      case 'i':
        read_int ("Digite a chave a ser inserida: ", strkey, UINTLEN);
        key = atoll (strkey);
        read_int ("Digite o valor a ser inserido: ", strkey, UINTLEN);
        value = atoll (strvalue);
        bptree_insert (tree, key, value);
        break;

      case 'r':
        printf ("Nao implementado.\n");
        break;

      case 'b':
        printf ("Nao implementado.\n");
        break;

      case 's':
        halt = 1;
        break;
   
      default:
        break;
    }
  } while (!halt);
  
  bptree_free (tree);

  return 0;
}
