#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "html.h"
#include "io.h"

void
html_begin(FILE *htmlfile)
{
  if (htmlfile)
    {
      fprintf (htmlfile, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
      fprintf (htmlfile, "<html>\n");
      fprintf (htmlfile, "<head>\n");
      fprintf (htmlfile, "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
      fprintf (htmlfile, "  <title>TP3 Grupo 1</title>\n");
      fprintf (htmlfile, "</head>\n\n");
      fprintf (htmlfile, "<body>\n");
      fprintf (htmlfile, "<table width=\"100%\" border=\"1\">\n");
    }
}

void
html_end(FILE *htmlfile)
{
  if (htmlfile)
    {
      fprintf (htmlfile, "</table>\n");
      fprintf (htmlfile, "</body>\n");
      fprintf (htmlfile, "</html>\n");
    }
}

void
html_write_record_info (FILE *htmlfile, ArtworkInfo *info)
{
  char *path = baseGetValidImagePath (info->img);

  assert (htmlfile && info);

  fprintf (htmlfile, "<tr>\n");
  fprintf (htmlfile, "<td>\n");
  fprintf (htmlfile, "<img src=\"img/%s\">\n", path);
  fprintf (htmlfile, "</td>\n");
  fprintf (htmlfile, "<td>\n");
  fprintf (htmlfile, "<p><b>Titulo:</b> %s</p>\n", info->title);
  fprintf (htmlfile, "<p><b>Tipo:</b> %s</p>\n", info->type);
  fprintf (htmlfile, "<p><b>Autor:</b> %s</p>\n", info->author);
  fprintf (htmlfile, "<p><b>Ano:</b> %s</p>\n", info->year);
  fprintf (htmlfile, "<p><b>Valor:</b> %s</p>\n", info->value);
  fprintf (htmlfile, "<p><b>Identificador:</b> %s</p>\n\n", info->img);
  fprintf (htmlfile, "</td>\n");
  fprintf (htmlfile, "</tr>\n");

  free (path);
}
