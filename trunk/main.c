#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "file.h"
#include "html.h"
#include "io.h"
#include "menu.h"
#include "pk.h"

#define DBFILE   "base01.dat"   /* The database file name. */
#define HTMLFILE "lista.html"   /* The HTML output. */
#define PKFILE   "pkfile.pk"    /* The primary key file name. */

int main(int argc, char* argv[]) {
  FILE *base;               /* base01.dat basically */
  FILE *htmlfile;           /* Every single report will be printed here */
  FILE *pkfile;             /* File with the primary key table. */

  artwork_info info;        /* Holds the artwork data. */
  PrimaryKeyList* pkindex;

  char c;                   /* Holds the user's choice from the menus. */
  int insert_data = 1;      /* Whether or not to insert more data into the dat file. */
  char name[NAME_LENGTH+1]; /* Holds the name for which to search. */
  int i;                    /* Number of entries in our database. */
  int match_pos;
 
  pkindex = pkListInit();
  if (pkindex == NULL) {
    printf("Erro ao carregar chaves primarias. Saindo.\n");
    exit(EXIT_FAILURE);
  }

  base = fopen(DBFILE, "r+");

  /* Loading the primary key tables. */
  printf("Carregando tabela de chaves primarias...\n");
  if (!fileExists(PKFILE) && fileExists(DBFILE)) {
    printf("A tabela de chaves primarias esta sendo criada.\n");
    pkListLoadFromBase(pkindex, base);
  }
  else if (fileExists(PKFILE) && fileExists(DBFILE)) {
    pkfile = fopen(PKFILE, "r");
    pkListLoadFromPK(pkindex, pkfile);
    fclose(pkfile);
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
      while (insert_data) {
        readData(&info);

        if (pkListFindByName(pkindex, info.title) != -1) {
          printf("\nJa existe uma obra com titulo \"%s\".\n", info.title);
          continue;
        }

        writeData(base, &info);
        pkListInsert(pkindex, info.title);

        while (1) {
          printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");

          if (readChar(&c) == -1)
            printf("\nOpcao invalida");

          c = tolower(c);

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

      break;

    case 'c':
      readString("\n    Por favor, digite o titulo da obra (Max: 200 caracteres): ",
                 name, NAME_LENGTH);
      match_pos = pkListFindByName(pkindex, name);

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

      if (pkListIsEmpty(pkindex)) {
        printf("     O catalogo ainda nao possui obras.\n");
      }
      else {
        htmlfile = fopen(HTMLFILE, "w");
        htmlBegin(htmlfile);

        fseek(base, 0, SEEK_SET);

        for (i = 0; i < pkindex->regnum; i++) {
          fseek(base, (pkindex->pklist[i].rrn) * REG_SIZE, SEEK_SET);
          readArtworkRecord(base, &info);
          htmlWriteRecordInfo(htmlfile, &info);
        }

        htmlEnd(htmlfile);
        fclose(htmlfile);

        printf("   Lista \"%s\" gerada com sucesso.\n", HTMLFILE);
      }

      break;

    case 's':
      printf("Salvando tabela de chaves de busca...\n");

      pkfile = fopen(PKFILE, "w");
      pkListWriteToFile(pkindex, pkfile);
      fclose(pkfile);

      pkListFree(pkindex);

      printf("Saindo...\n");
      return 0;
    }
  }

  return 0;
}
