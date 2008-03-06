#include "io.h"
#include "menu.h"

int main(int argc, char* argv[]) {
	FILE *file;
	artwork_info info;    /* Holds the artwork data. */
	char c;               /* Holds the user's choice from the menus. */
  int insert_data = 1;  /* Whether or not to insert more data into the dat file */

	printWelcome();

  while (1) {
    printMenu();

    c = getchar();
    flushBuffer();

		switch (c) {
    default:
      printf("\n(%c): Opcao invalida.\n", c);
      break;
    case 'i':
			/* Open the file for appending. */
			file = fopen("base01.dat", "a");

      while (insert_data) {
        readData(&info);
        writeData(file, &info);

        while (1) {
          printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? "); 
          c = getchar();
          flushBuffer();

          /* We only have two options, there's no
           * need for a switch here */
          if (c == 's')
            break;
          else if (c == 'n') {
            insert_data = 0;
            break;
          } else
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
