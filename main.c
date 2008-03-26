#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "io.h"
#include "menu.h"

int main(int argc, char* argv[]) {

  FILE *file;
  FILE *html;
  artwork_info info;    /* Holds the artwork data. */
  char c;               /* Holds the user's choice from the menus. */
  char input[2];        /* Holds the full user input from the menu. */
  int insert_data = 1;  /* Whether or not to insert more data into the dat file. */
  char name[NAME_LENGTH];


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
      file = fopen("base01.dat", "a");
      
      while (insert_data) {
	readData(&info);
	writeData(file, &info);
	  
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

      fclose(file);
      break;
      
    case 'c':
      readString("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ",
		 name, NAME_LENGTH);
      /*search for it in the array....*/
      /* do something */
      break;

    case 'g':
      printf("Gerando lista de obras...\n");
      html = fopen("list.html", "w");
      file = fopen("base01.dat", "a");
      if(makeHtml(file, html))
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
      printf("Saindo...\n");
      return 0;
    }
  }

  return 0;
}
