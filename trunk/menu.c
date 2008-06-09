#include <stdio.h>
#include <string.h>
#include "io.h"
#include "menu.h"

char
menuMultipleAnswers (const char *msg, const char *optstring)
{
  char c;

  while (1)
    {
      printf (msg);

      if ((readChar (&c)) || (!strchr (optstring, c)))
        {
          printf ("\n   Opcao invalida.\n");
          continue;
        }

      return c;
    }
}

int
menuYesOrNo (const char *msg)
{
  return menuMultipleAnswers (msg, "sn") == 's';
}

void
printConsultMenu (void)
{
  printf ("\n");
  printf ("   ------------------------------------\n");
  printf ("   Efetuar consulta por:\n");
  printf ("     (a)no da obra\n");
  printf ("     a(u)tor da obra\n");
  printf ("     t(i)po da obra\n");
  printf ("     (t)itulo da obra\n");
  printf ("\n");
  printf ("   A busca sera por apenas uma palavra.\n");
  printf ("   ------------------------------------\n");
  printf ("\n");
}

void
print_search_type_menu (void)
{
  printf ("\n");
  printf ("   +++++++++++++++++++++++++++++\n");
  printf ("   Tipo de consulta:\n");
  printf ("   (e)xata, por titulo\n");
  printf ("   (p)arcial, por qualquer campo\n");
  printf ("   (s)imilaridade\n");
  printf ("   +++++++++++++++++++++++++++++\n");
  printf ("\n");
}

void
printMainMenu (void)
{
  printf ("\n");
  printf ("    _____________________________________\n");
  printf ("   |                                     |\n");
  printf ("   |  (i)nserir entrada no catalogo      |\n");
  printf ("   |  (r)emover entrada do catalogo      |\n");
  printf ("   |  (c)onsultar obra de arte           |\n");
  printf ("   |  (s)air                             |\n");
  printf ("   |_____________________________________|\n\n");
}

void
printWelcome (void)
{
  printf ("\n");
  printf ("   ########################################\n");
  printf ("   ##                                    ##\n");
  printf ("   ##          MC326 1s2008              ##\n");
  printf ("   ##    GERENCIAMENTO DE OBRAS DE ARTE  ##\n");
  printf ("   ##                                    ##\n");
  printf ("   ########################################\n\n");
  printf ("=================================================\n");
}
