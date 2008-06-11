#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adapter.h"
#include "menu.h"

int
main (int argc, char *argv[])
{
  /* We just need this struct, since  it will hold all the others. */
  Adapter *db;

  /* Allocate it. */
  db = adapter_new ();

  /*Try loading it from the files. */
  adapter_load_files (db);

  printWelcome ();

  while (1)
    {
      printMainMenu ();

      switch (menuMultipleAnswers ("   Opcao desejada: ", "cigrs"))
        {
        default:               /* This should not happen */
          break;
        case 'c':
          adapter_find (db);
          break;
        case 'g':
          adapter_list (db);
          break;
        case 'i':
          adapter_insert (db);
          break;
        case 'r':
          adapter_remove (db);
          break;
        case 's':
          adapter_free (db);
          printf ("Saindo...\n");
          return 0;
        }
    }

  return 0;
}