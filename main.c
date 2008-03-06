#include "io.h"
#include "menu.h"

int main() {
  FILE *file;
  artwork_info info; /* Holds the artwork data. */
  char c; /* Holds the user's choice from the menus. */

  printWelcome();

  printMenu();

  while ( scanf("%c", &c) ) {
    getchar(); /* There is always a '\n' after the input, so take it.*/
    switch (c) {
    case '\n':
      /* Sometimes getchar() gets the wrong letter... so try again! */
      break;
    case 'i':
      /* Open the file for appending. */
      file = fopen("base01.dat", "a");
      readData(&info);
      writeData(file, &info);
      printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");

      while ( scanf("%c", &c) && c == 's') {
	getchar();
	readData(&info);
	getchar();
	writeData(file, &info);
	printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");
      }
      getchar();
      fclose(file);
      break;
    case 's':
      printf("Saindo...\n");
      return 0;
    }
    printMenu();
  }

  return 0;
}
