#include "io.h"
#include "menu.h"

int main()
{
        FILE *file;
        obra_info info; /* Holds the artwork data. */
        char c; /* Holds the user's choice from the menus. */

        printWelcome();

        printMenu();

        while ( scanf("%c", &c) ) {
                getchar();
                switch (c) {
                case '\n':
                        /* Sometimes getchar() gets the wrong letter... so try again! */
                        break;
                case 'i':
                        /* Open the file for appending. */
                        file = fopen("base01.dat", "a");
                        read_data(&info);
                        /* read_data() leaves a '\n' behind, so getchar() gets it. */
                        getchar();
                        write_data(file, &info);
                        printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");

                        while ( scanf("%c", &c) && c == 's') {
                                getchar();
                                read_data(&info);
                                getchar();
                                write_data(file, &info);
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
