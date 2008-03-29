#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "html.h"
#include "io.h"

void htmlBegin(FILE* htmlfile) {
  if (htmlfile) {
    fprintf(htmlfile, "<html>\n");
    fprintf(htmlfile, "<head>\n");
    fprintf(htmlfile, "  <title>TP2 Grupo 1</title>\n");
    fprintf(htmlfile, "</head>\n\n");
    fprintf(htmlfile, "<body>\n");
  }
}

void htmlEnd(FILE* htmlfile) {
  if (htmlfile) {
    fprintf(htmlfile, "</body>\n");
    fprintf(htmlfile, "</html>\n");
  }
}

void htmlWriteRecordInfo(FILE* htmlfile, artwork_info* info) {
  char* path = getValidImagePath(info->img);

  if (htmlfile && info) {
    fprintf(htmlfile, "<img src=\"img/%s\">", path);
    fprintf(htmlfile, "<p><b>Titulo: %s</b></p>\n", info->title);
    fprintf(htmlfile, "<p><b>Tipo: %s</b></p>\n", info->type);
    fprintf(htmlfile, "<p><b>Autor: %s</b></p>\n", info->author);
    fprintf(htmlfile, "<p><b>Ano: %04d</b></p>\n", info->year);
    fprintf(htmlfile, "<p><b>Valor: %012d</b></p>\n", info->value);
    fprintf(htmlfile, "<p><b>Identificador: %s</b></p>\n", info->img);
  }

  free(path);

}