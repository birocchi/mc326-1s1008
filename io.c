#include "io.h"

int read_data(obra_info *info)
{

        if (!info) {
                return 1;
        }/* Caso o ponteiro seja nulo, retorna erro. */

        printf("Por favor digite o titulo da obra: ");
        fgets(((*info).title), 200, stdin);
        /*   scanf(" %s ", ((*info).title)); */
        printf("\nPor favor digite o tipo da obra: ");
        fgets(((*info).tipo), 100, stdin);
        /*   scanf(" %s ", ((*info).tipo)); */
        printf("\nPor favor digite o autor da obra: ");
        fgets(((*info).autor), 100, stdin);
        /*   scanf(" %s ", ((*info).autor)); */
        printf("\nPor favor digite o ano da obra: ");
        scanf("%d", &((*info).ano));
        printf("\nPor favor digite o valor da obra: ");
        scanf("%d", &((*info).valor));
        printf("\nPor favor digite o identificador da obra: ");
        scanf("%s", ((*info).img));

        return 0;
}

int write_data(FILE *file, obra_info *info)
{

        /* Return error if the file or struct pointers are NULL. */
        if (!file | !info) {
                return 1;
        }

        fprintf(file, "%-200s",   (*info).title);
        fprintf(file, "%-100s",   (*info).tipo);
        fprintf(file, "%-100s",   (*info).autor);
        fprintf(file, "%04d",      (*info).ano);
        fprintf(file, "%09d",    (*info).valor);
        fprintf(file, "%-7s",     (*info).img);

        return 0;
}
