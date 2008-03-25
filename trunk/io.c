#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

void flushBuffer(void) {
  int c;

  while ((c = getchar()) != '\n' && c != EOF)
    continue;
}


int makeHtml(FILE *base, FILE *html)
{
  if (!base || !html){
    return 1;
  }

  fprintf(html, "<html>\n<head>\n</head><body>\n");
  fprintf(html, "<p>teste: <img src=\"./img/012426.png\">\n");

  /* get base's size to know how many entries..
     then read that many entries part by part
     fprinting its data adequately.*/  

  fprintf(html, "</body>\n</html>\n");

  return 0;
}


int readData(artwork_info *info) {
  char img[IMG_LENGTH + 1];

  /* Returns an error if the pointer is NULL. */
  if (!info)
    return 1;

  readString("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ", info->title, NAME_LENGTH);
  readString("\n   Por favor, digite o tipo da obra (Max: 100 caracteres): ", info->type, TYPE_LENGTH);
  readString("\n   Por favor, digite o autor da obra (Max: 125 caracteres): ", info->author, AUTHOR_LENGTH);

  readInt("\n   Por favor, digite o ano da obra (Max: 4 caracteres): ", &(info->year), YEAR_LENGTH);
  readInt("\n   Por favor, digite o valor da obra (Max: 12 caracteres): ", &(info->value), VALUE_LENGTH);

  while (1) {
    readString("\n   Por favor, digite o identificador da obra (Max: 9 caracteres): ", img, IMG_LENGTH);

    /* Validate the image identifier */
    if (!validateIdentifier(img)) {
      strncpy(info->img, img, IMG_LENGTH + 1);
      break;
    }
    else {
      printf("   Entrada invalida.");
      continue;
    }
  }

  return 0;
}


void readInt(const char* inputText, int* dest, size_t length)
{
  /* In a 32-bit system, INT_MAX = 2147483647, which
   * has 10 digits. We can have a '-', which makes 11
   * characters.
   * 11 characters are probably enough for an integer.
   */
  char tmp[11+1];
  char* endptr;

  while (1) {
    printf(inputText);
    readValue(tmp, length);

    *dest = strtol(tmp, &endptr, 10);

    /* If it is a null entry or if there are non-digits, read it again */
    if ((endptr == tmp) || (*endptr != '\0')) {
      printf("   Entrada invalida.");
      continue;
    } else
      break;
  }
}


void readString(const char* inputText, char* dest, size_t length)
{
  while (1) {
    printf(inputText);
    readValue(dest, length);

    if (strlen(dest) == 0) {
      printf("   Entrada invalida.");
      continue;
    } else
      break;
  }
}


void readValue(char s[], size_t length) {
  /* fgets reads n-1 characters from the stream, so we
   * use length+1 to make readValue calls keep making sense. */
  fgets(s, length+1, stdin);

  if (!stripNewLine(s))
    flushBuffer();
}


int stripNewLine(char s[]) {
  int pos;

  for (pos = 0; pos < strlen(s); pos++) {
    if (s[pos] == '\n') {
      s[pos] = '\0';
      return 1;
    }
  }

  return 0;
}


int validateIdentifier(const char* name)
{
  char* endptr;
  int i;

  i = strtol(name, &endptr, 10);
  if ((endptr == name) || (endptr == '\0') || strlen(endptr) != 3 || ((strncmp(endptr, "jpg", 3)) &&
        strncmp(endptr, "gif", 3) && (strncmp(endptr, "png", 3))))
    return 1;
  else
    return 0;
}


int writeData(FILE *file, artwork_info *info) {

  /* Return error if the file or struct pointers are NULL. */
  if (!file || !info) {
    return 1;
  }

  fprintf(file, "%-200s",   info->title);
  fprintf(file, "%-100s",   info->type);
  fprintf(file, "%-125s",   info->author);
  fprintf(file, "%04d",     info->year);
  fprintf(file, "%012d",    info->value);
  fprintf(file, "%s",       info->img);

  return 0;
}
