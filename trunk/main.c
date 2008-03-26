#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "io.h"
#include "menu.h"
#include "db.h"

int main(int argc, char* argv[]) {

  FILE *base; /* base01.dat basically*/
  FILE *html; /* Every single report will be printed here */
  FILE *pkfile; /* File with the primary key table. */ 
  artwork_info info;    /* Holds the artwork data. */
  char c;               /* Holds the user's choice from the menus. */
  char input[2];        /* Holds the full user input from the menu. */
  int insert_data = 1;  /* Whether or not to insert more data into the dat file. */
  char name[NAME_LENGTH]; /* Holds the name for which to search. */
  char ** pkindex; /* Primary Key search table. */
  int i, numreg; /* Number of entries in our database. */
  char **find;
  
  /* Load database for size-check. */
  base = fopen("base01.dat", "r");
  numreg = getFileSize(base) / REG_SIZE;
  fclose(base);

  /* Allocating some memory for our PK table. */
  pkindex = (char**)malloc(sizeof(char*) * numreg);
  for( i = 0; i < numreg; i++){
    pkindex[i] = (char*)malloc(sizeof(char) * 210);
  }

  /* Loading the primary key tables. */
  printf("Carregando tabela de chaves primárias...\n");
  pkfile = fopen("pkfile.pk", "r");
  if(!pkfile){
    printf("Arquivo de chaves primárias não pode ser carregado.\n");
    makeArrayPKIndex(pkindex, base);
  }
  else{
    loadPkFile(pkindex, pkfile);
  }
  fclose(pkfile);


  printWelcome();

  while (1) {
    printMenu();

    /* We read n+1 from the input to be able to check
     * if the user has written exactly n characters */
    readValue(input, 2);
    if (strlen(input) != 1) {
      printf("\nErro: Opcao invalida.\n");
      continue;
    }

    switch (tolower(input[0])) {
    default:
      printf("\n(%c): Opcao invalida.\n", c);
      break;
      
    case 'i':
      /* Open the file for appending. */
      insert_data = 1;
      base = fopen("base01.dat", "a");
      
      while (insert_data) {
	readData(&info);
	writeData(base, &info);
	  
	while (1) {
	  printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? "); 
	  c = tolower(getchar());
	  flushBuffer();

	  if (c == 's')
	    break;
	  else if (c == 'n') {
	    insert_data = 0;
	    break;
	  } 
	  else
	    printf("\nOpcao invalida");
	}
      }

      fclose(base);
      break;
      
    case 'c':
      readString("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ",
		 name, NAME_LENGTH);
      find = search(pkindex, name, numreg);
      printf("%s\n", *find);
      /* do something */
      break;

    case 'g':
      printf("Gerando lista de obras...\n");
      html = fopen("list.html", "w");
      base = fopen("base01.dat", "a");
      if(makeHtml(base, html))
	printf("Erro ao gerar lista de obras!\n");
      else{
	printf("Lista gerada com sucesso.\n");
	printf("Deseja vizualisar a lista em seu browser? (s)im, (n)ao? ");
	c = tolower(getchar());
	flushBuffer();

	if (c == 's'){
	  system("firefox list.html &");
	  break;
	}
	else if (c == 'n') {
	  break;
	} 
	else
	  printf("\nOpcao invalida");
      }
      
      break;

    case 's':
      printf("Salvando tabela de chaves de busca...\n");
      pkfile = fopen("pkfile.pk", "w");
      makeFilePKIndex(pkindex, pkfile, numreg);
      fclose(pkfile);
      
      for(i = 0; i < numreg; i++){
	free(pkindex[i]);
      }
      free(pkindex);

      printf("Saindo...\n");
      return 0;
    }
  }

  return 0;
}
