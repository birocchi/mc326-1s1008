#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "base.h"
#include "file.h"
#include "html.h"
#include "io.h"
#include "menu.h"
#include "pk.h"

#define DBFILE   "base01.dat"   /* The database file name. */
#define HTMLFILE "lista.html"   /* The HTML output. */
#define PKFILE   "pkfile.pk"    /* The primary key file name. */

int main(int argc, char* argv[]) {
  FILE *base;                 /* base01.dat basically */
  FILE *htmlfile;             /* Every single report will be printed here */
  FILE *pkfile;               /* File with the primary key table. */

  artwork_info info;          /* Holds the artwork data. */
  PrimaryKeyList* pkindex;

  char c;                     /* Holds the user's choice from the menus. */
  int insert_data = 1;        /* Whether or not to insert more data into the dat file. */
  char name[TITLE_LENGTH+1];  /* Holds the name for which to search. */
  int i;                      /* Number of entries in our database. */
  int match_pos;              /* Will hold the rrn of the found register. */

  /* Try to open it for reading. */
  base = fopen(DBFILE, "r");
  if (!base) {
    printf("Base de dados ainda nao existe.\n");
  }

  pkindex = pkListInit();
  if (pkindex == NULL) {
    printf("Erro ao carregar chaves primarias. Saindo.\n");
    exit(EXIT_FAILURE);
  }

  /* Loading the primary key tables. */
  printf("Carregando tabela de chaves primarias...\n");
  if (fileExists(PKFILE) && fileExists(DBFILE)) {
    pkfile = fopen(PKFILE, "r");
    pkListLoadFromPK(pkindex, pkfile);
    fclose(pkfile);
  }
  else if (fileExists(DBFILE)){
    printf("A tabela de chaves primarias esta sendo criada.\n");
    pkListLoadFromBase(pkindex, base);
  }

  /* If there was a database, we now close it. */
  if (base)
    fclose(base);

  /* And now we can open it for appending so we can insert new registers. */
  base = fopen(DBFILE, "a+");
  if (!base) {
    printf("Erro ao criar base de dados. Saindo...\n");
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
      insert_data = 1;

      while (insert_data) {
        readData(&info);

        if (pkListFindByName(pkindex, info.title) != -1) {
          printf("\nJa existe uma obra com titulo \"%s\".\n", info.title);
          continue;
        }

        baseWriteData(base, &info);
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
                 name, TITLE_LENGTH);
      /* Getting the rrn of the search. */
      match_pos = pkListFindByName(pkindex, name);

      if (match_pos == -1)
        printf("\n    Nao foi encontrada nenhuma obra com titulo \"%s\".\n", name);
      else {
        htmlfile = fopen(HTMLFILE, "w");
        htmlBegin(htmlfile);

        /* Go to the database position of the found rrn. */
        fseek(base, match_pos*REG_SIZE, SEEK_SET);
        /* And then read that register. */
        baseReadArtworkRecord(base, &info);
        /* So we can write it to the html file. */
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

        /* For each pk in the index, start from the beggining since it's sorted. */
        for (i = 0; i < pkindex->regnum; i++) {
          /* Seek it into the database. */
          fseek(base, (pkindex->pklist[i].rrn) * REG_SIZE, SEEK_SET);
          /* Read it's contents. */
          baseReadArtworkRecord(base, &info);
          /* And write it to the html file. */
          htmlWriteRecordInfo(htmlfile, &info);
        }

        htmlEnd(htmlfile);
        fclose(htmlfile);

        printf("   Lista \"%s\" gerada com sucesso.\n", HTMLFILE);
      }

      break;

    case 's':
      printf("Salvando tabela de chaves de busca...\n");

      /* We always rewrite the pkfile since it must be sorted. */
      pkfile = fopen(PKFILE, "w");
      pkListWriteToFile(pkindex, pkfile);
      fclose(pkfile);

      /* Free the alocated memory for the PK index. */
      pkListFree(pkindex);
      fclose(base);

      printf("Saindo...\n");
      return 0;
    }
  }

  return 0;
}
