#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "db.h"
#include "file.h"
#include "html.h"
#include "io.h"
#include "menu.h"

#define DBFILE "base01.dat" /* The database file name. */
#define HTMLFILE "lista.html"
#define PKFILE "pkfile.pk"  /* The primary key file name. */

int main(int argc, char* argv[]) {
  FILE *base;             /* base01.dat basically*/
  FILE *htmlfile;             /* Every single report will be printed here */
  FILE *pkfile;           /* File with the primary key table. */

  artwork_info info;      /* Holds the artwork data. */

  char c;                 /* Holds the user's choice from the menus. */
  int insert_data = 1;    /* Whether or not to insert more data into the dat file. */
  char name[NAME_LENGTH+1]; /* Holds the name for which to search. */
  int i, numreg = 0;      /* Number of entries in our database. */
  int match_pos;
  primary_key* pk_index;

  base = fopen(DBFILE, "r+");

  /* Loading the primary key tables. */
  printf("Carregando tabela de chaves primarias...\n");
  if (!fileExists(PKFILE) && fileExists(DBFILE)) {
    printf("A tabela de chaves primarias esta sendo criada.\n");
    pk_index = createPKFromBase(base, &numreg);
  }
  else if (!fileExists(PKFILE) && !fileExists(DBFILE)) {
    pk_index = createFirstPK();
  }
  else {
    pkfile = fopen(PKFILE, "r");
    pk_index = loadPKFile(pkfile, &numreg);
    fclose(pkfile);
  }

  if (pk_index == NULL) {
    printf("Erro ao carregar chaves primarias. Saindo.\n");
    exit(EXIT_FAILURE);
  }

  printWelcome();

  while (1) {
    printMenu();

    if (readChar(&c)) {
      printf("\nErro: Opcao invalida.\n");
      continue;
    }

    switch (tolower(c)) {
    default:
      printf("\n(%c): Opcao invalida.\n", c);
      break;

    case 'i':
#if 0
      /* Open the file for appending. */
      insert_data = 1;
      base = fopen("base01.dat", "a");

      while (insert_data) {
        readData(&info);
        writeData(base, &info);
        pk_index = incrementPK(pk_index, numreg, &info);
        numreg++;

        while (1) {
          printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");

          c = tolower(readChar());
          if (c == -1)
            printf("\nOpcao invalida");

          if (c == 's')
            break;
          else if (c == 'n') {
            insert_data = 0;
            if (base)
              fclose(base);
            break;
          }
          else
            printf("\nOpcao invalida");
        }
      }
#endif
      break;

    case 'c':
      readString("\n    Por favor, digite o titulo da obra (Max: 200 caracteres): ",
                 name, NAME_LENGTH);
      match_pos = findEntry(pk_index, name, numreg);

      if (match_pos == -1)
        printf("\n    Nao foi encontrada nenhuma obra com titulo \"%s\".\n", name);
      else {
        htmlfile = fopen(HTMLFILE, "w");
        htmlBegin(htmlfile);

        fseek(base, match_pos*REG_SIZE, SEEK_SET);
        readArtworkRecord(base, &info);

        htmlWriteRecordInfo(htmlfile, &info);

        htmlEnd(htmlfile);
        fclose(htmlfile);

        printf("\n    As informacoes da obra \"%s\" foram salvas em \"%s\".\n", name, HTMLFILE);
      }

      break;

    case 'g':
      printf("   Gerando lista de obras...\n");

      /* TODO: Essa verificacao precisa melhorar ou nao existir */
      if (!base) {
        printf("     Nao existe nenhuma obra ainda.\n");
      }
      else {
        htmlfile = fopen(HTMLFILE, "w");
        htmlBegin(htmlfile);

        fseek(base, 0, SEEK_SET);

        for (i = 0; i < numreg; i++) {
          fseek(base, (pk_index[i].rrn) * REG_SIZE, SEEK_SET);
          readArtworkRecord(base, &info);
          htmlWriteRecordInfo(htmlfile, &info);
        }

        htmlEnd(htmlfile);
        fclose(htmlfile);

        printf("   Lista gerada com sucesso.\n");
      }

      break;

    case 's':
      printf("Salvando tabela de chaves de busca...\n");

      pkfile = fopen(PKFILE, "w");
      writePKToFile(pk_index, pkfile, numreg);
      fclose(pkfile);

      free(pk_index);

      printf("Saindo...\n");
      return 0;
    }
  }

  return 0;
}
