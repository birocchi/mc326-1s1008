#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "html.h"
#include "io.h"

void htmlBegin(FILE* htmlfile) {
  if (htmlfile) {
    fprintf(htmlfile, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
    fprintf(htmlfile, "<html>\n");
    fprintf(htmlfile, "<head>\n");
    fprintf(htmlfile, "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
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
  char* path = baseGetValidImagePath(info->img);

  if (htmlfile && info) {
    fprintf(htmlfile, "<img src=\"img/%s\">", path);
    fprintf(htmlfile, "<p><b>Titulo: %s</b></p>\n", info->title);
    fprintf(htmlfile, "<p><b>Tipo: %s</b></p>\n", info->type);
    fprintf(htmlfile, "<p><b>Autor: %s</b></p>\n", info->author);
    fprintf(htmlfile, "<p><b>Ano: %s</b></p>\n", info->year);
    fprintf(htmlfile, "<p><b>Valor: %s</b></p>\n", info->value);
    fprintf(htmlfile, "<p><b>Identificador: %s</b></p>\n", info->img);
  }

  free(path);

}
