#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adapter.h"
#include "menu.h"

int
main (int argc, char *argv[])
{
  Adapter *db;
  int halt = 0;

  db = adapter_new ();

  adapter_load_files (db);

  printWelcome ();

  while (!halt)
    {
      printMainMenu ();

      switch (menuMultipleAnswers ("   Opcao desejada: ", "cirs"))
        {
        case 'c':
          adapter_find (db);
          break;
        case 'i':
          adapter_insert (db);
          break;
        case 'r':
          adapter_remove (db);
          break;
        case 's':
          printf ("Saindo...\n");
          halt = 1;
          break;
        }
    }

  adapter_free (db);

  return 0;
}
