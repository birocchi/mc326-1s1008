#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "db.h"
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

  char input[2];
  char c = 0;                 /* Holds the user's choice from the menus. */
  int insert_data = 1;    /* Whether or not to insert more data into the dat file. */

  char name[NAME_LENGTH]; /* Holds the name for which to search. */
  int i, numreg;          /* Number of entries in our database. */
  int match_pos;

  primary_key* pk_index;

  base = fopen(DBFILE, "r+");

  /* Loading the primary key tables. */
  printf("Carregando tabela de chaves primarias...\n");
  if (!fileExists(PKFILE)) {
    printf("A tabela de chaves primarias esta sendo criada.\n");
    pk_index = createPKFromBase(base, &numreg);
  } else {
    pkfile = fopen(PKFILE, "r");
    pk_index = loadPKFile(pkfile, &numreg);
    fclose(pkfile);
  }

  printWelcome();

  while (1) {
    printMenu();

#if 0
    c = readChar();
    if (c == -1) {
      printf("\nErro: Opcao invalida.\n");
      continue;
    }
#endif
      /* We read n+1 from the input to be able to check
     * if the user has written exactly n characters */
    readValue(input, 2);
    if (strlen(input) != 1) {
      printf("\nErro: Opcao invalida.\n");
      continue;
    }


    switch (tolower(input[0])) {
    default:
      printf("\n(%c): Opcao invalida.\n", input[0]);
      break;

    case 'i':
      /* Open the file for appending. */
      insert_data = 1;
      base = fopen("base01.dat", "a");

      while (insert_data) {
        readData(&info);
        writeData(base, &info);
/* 	numreg++; */
	pk_index = incrementPK(pk_index, numreg, &info);

        while (1) {
          printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");

          c = tolower(readChar());
          if (c == -1)
            printf("\nOpcao invalida");

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
      }

      break;

    case 'g':
      printf("   Gerando lista de obras...\n");
      
      if(!base){
	printf("     Nao existe nenhuma obra ainda.\n");
      }
      else{
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
      /*  printf("   Deseja vizualisar a lista em seu browser? (s)im, (n)ao? "); */
/*
      while (1) {
        c = readChar();
        if (c == -1)
          printf("\n   Opcao invalida\n");

        if (c == 's') {
          system("firefox list.html &");
          break;
        }
        else if (c == 'n')
          break;
        else
          printf("\n   Opcao invalida\n");
      }*/


      break;

    case 's':
      printf("Salvando tabela de chaves de busca...\n");

      pkfile = fopen(PKFILE, "w");
      writePKToFile(pk_index, pkfile, numreg);

      free(pk_index);
      fclose(pkfile);

      if(base)
	fclose(base);

      printf("Saindo...\n");
      return 0;
    }
  }

  return 0;
}
