#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

void flushBuffer(void)
{
  int c;

  while ((c = getchar()) != '\n' && c != EOF)
    continue;
}

int readChar(char* c) {
  char input[3];

  /* We read n+1 from the input to be able to check
   * if the user has written exactly n characters */
  readValue(input, 2);

  if (strlen(input) != 1)
    return 1;
  else {
    *c = input[0];
    return 0;
  }
}

int readData(artwork_info *info)
{
  char img[IMG_LENGTH+1];

  /* Returns an error if the pointer is NULL. */
  if (!info)
    return 1;

  readString("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ",
             info->title, TITLE_LENGTH);
  readString("\n   Por favor, digite o tipo da obra (Max: 100 caracteres): ",
             info->type, TYPE_LENGTH);
  readString("\n   Por favor, digite o autor da obra (Max: 125 caracteres): ",
             info->author, AUTHOR_LENGTH);

  readInt("\n   Por favor, digite o ano da obra (Max: 4 caracteres): ",
          info->year, YEAR_LENGTH);
  readInt("\n   Por favor, digite o valor da obra (Max: 12 caracteres): ",
          info->value, VALUE_LENGTH);

  while (1) {
    readString("\n   Por favor, digite o identificador da obra (Max: 9 caracteres): ",
               img, IMG_LENGTH);

    /* Validate the image identifier */
    if (!baseIsValidIdentifier(img)) {
      strncpy(info->img, img, IMG_LENGTH+1);
      break;
    }
    else {
      printf("   Entrada invalida.");
      continue;
    }
  }

  return 0;
}

void readInt(const char* inputText, char* dest, size_t length)
{
  int i, invalid;

  invalid = 1;

  while (invalid) {
    readString(inputText, dest, length);

    for (i = 0; i < strlen(dest); i++) {
      if (!isdigit(dest[i])) {
        printf("Entrada invalida.\n");
        invalid = 1;
        break;
      }
    }

    invalid = 0;
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

void readValue(char s[], size_t length)
{
  fgets(s, length+1, stdin);

  stripNewLine(s);
  stripWhiteSpace(s);
}

void stripNewLine(char s[])
{
  char* pos = strchr(s, '\n');

  if (!pos)
    flushBuffer();
  else
    *pos = '\0';
}

void stripWhiteSpace(char str[]) {
  int i, j, pos, space;
  int len;
  char* buffer;

  len = strlen(str);
  if (len < 1)
    return;

  buffer = (char*)calloc(len, sizeof(char));
  if (buffer) {
    for (i = 0; i < len && isspace(str[i]); i++);
    for (j = len-1; j > i && isspace(str[j]); j--);

    space = 0;
    pos = 0;
    for (; i <= j; i++) {
      if (isspace(str[i]) && !space) {
        buffer[pos++] = str[i];
        space = 1;
      }
      else if (isspace(str[i]) && space)
        pos++;
      else {
        buffer[pos++] = str[i];
        space = 0;
      }
    }

    strncpy(str, buffer, pos);
    free(buffer);
  }
}
