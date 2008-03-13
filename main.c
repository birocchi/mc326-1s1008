#include <ctype.h>
#include <string.h>
#include "data.h"
#include "io.h"
#include "menu.h"

int main(int argc, char* argv[]) {

  FILE *file;
  artwork_info info;    /* Holds the artwork data. */
  char c;               /* Holds the user's choice from the menus. */
  char input[2];        /* Holds the full user input from the menu. */
  int insert_data = 1;  /* Whether or not to insert more data into the dat file. */

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

            /* We only have two options, there's no
             * need for a switch here */
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
	
      case 's':
        printf("Saindo...\n");
        return 0;
    }
  }

  return 0;
}
