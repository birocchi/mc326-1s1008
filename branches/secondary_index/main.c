#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "avail.h"
#include "base.h"
#include "file.h"
#include "filelist.h"
#include "html.h"
#include "io.h"
#include "menu.h"
#include "pk.h"

int main(int argc, char* argv[]) {
  FILE *base;                 /* base01.dat basically */
  FILE *htmlfile;             /* Every single report will be printed here */
  FILE *pkfile;               /* File with the primary key table. */
  FILE *availfile;            /* Open the avail list head. */

  ArtworkInfo info;          /* Holds the artwork data. */
  PrimaryKeyList* pkindex;

  char name[TITLE_LENGTH+1];  /* Holds the name for which to search. */
  int i;                      /* Number of entries in our database. */
  int match_pos;              /* Will hold the rrn of the found register. */
  int avail;                  /* Holds the RRN of the first available spot */
  int rrn;                    /* Hold the removed field RRN. */

  availfile = fopen(AVAIL_HEAD, "r");
  if (!availfile){
    printf("Avail list nao existe, inicializando com -1.\n");
    availfile = fopen(AVAIL_HEAD, "w");
    fprintf(availfile, "-1");
    fclose(availfile);
    avail = -1;
  }
  else{
    printf("Carregando primeira posicao disponivel. ");
    fscanf(availfile, "%d", &avail);
    fclose(availfile);
  }

  pkindex = pkListLoad(DBFILE, PKFILE);

  /* Open the files */
  base = fopen(DBFILE, "a+");
  assert(base != NULL);

  open_si_files(&si_files);

  printWelcome();

  while (1) {
    printMainMenu();

    switch (menuMultipleAnswers("   Opcao desejada: ", "cigrs")) {
    default: /* This should not happen */
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

      if (pkListIsEmpty(pkindex))
        printf("     O catalogo ainda nao possui obras.\n");
      else {
        htmlfile = fopen(HTMLFILE, "w");
        htmlBegin(htmlfile);

        fseek(base, 0, SEEK_SET);

        /* For each pk in the index, start from the beggining since it's sorted. */
        for (i = 0; i < pkindex->regnum; i++) {
          /* Seek it into the database. */
          fseek(base, (pkindex->pklist[i].rrn) * BASE_REG_SIZE, SEEK_SET);
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
    case 'i':
      do {
        readData(&info);
        if (pkListInsert(pkindex, info.title)) {
          printf("\nJa existe uma obra com titulo \"%s\".\n", info.title);
          continue;
        }
        baseWriteData(base, &info);
      } while (menuYesOrNo("\nDeseja inserir mais uma entrada? (s)im, (n)ao? "));
      break;
    case 'r':
      readString("\n    Por favor, digite o titulo da obra (Max: 200 caracteres): ",
                 name, TITLE_LENGTH);

      if(pkListRemove(pkindex, name, &rrn)){
        printf("\n    Obra \"%s\" nao encontrada.\n", name);
      }
      else{
        removedField(base, rrn, &avail);
        printf("\n    Obra \"%s\" removida com sucesso.\n", name);
      }
      break;
    case 's':
      printf("Salvando tabela de chaves de busca...\n");

      availfile = fopen(AVAIL_HEAD, "w");
      fprintf(availfile, "%09d", avail);

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
