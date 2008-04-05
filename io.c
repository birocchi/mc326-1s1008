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
  char input[2+1];

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
  /* We use IMG_LENGTH-2 here because we exclude the first
   * two characters in the identifier (the group number)
   * since it's constant.
   */
  char img[ (IMG_LENGTH-2)+1 ];

  /* Returns an error if the pointer is NULL. */
  if (!info)
    return 1;

  readString("\n   Por favor, digite o titulo da obra (Max: 200 caracteres): ",
             info->title, TITLE_LENGTH);
  readString("   Por favor, digite o tipo da obra (Max: 100 caracteres): ",
             info->type, TYPE_LENGTH);
  readString("   Por favor, digite o autor da obra (Max: 125 caracteres): ",
             info->author, AUTHOR_LENGTH);

  readInt("   Por favor, digite o ano da obra (Max: 4 caracteres): ",
          info->year, YEAR_LENGTH);
  readInt("   Por favor, digite o valor da obra (Max: 12 caracteres): ",
          info->value, VALUE_LENGTH);

  while (1) {
    readString("   Por favor, digite o identificador da obra (Max: 7 caracteres): ",
               img, IMG_LENGTH-2);

    /* Validate the image identifier */
    if (!baseIsValidIdentifier(img)) {
      strncpy(info->img, GROUP_NUMBER, 2);
      strncpy(info->img+2, img, (IMG_LENGTH-2)+1);
      break;
    }
    else {
      printf("   Entrada invalida.");
      continue;
    }
  }

  return 0;
}

/* This is basically a wrapper around readString
 * which checks if there are only digits in the
 * string read and loops back if not.
 */
void readInt(const char* inputText, char* dest, size_t length)
{
  int i, invalid;

  invalid = 1;

  while (invalid) {
    invalid = 0;
    readString(inputText, dest, length);

    for (i = 0; i < strlen(dest); i++) {
      if (!isdigit(dest[i])) {
        printf("Entrada invalida.\n");
        invalid = 1;
        break;
      }
    }
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
  int i, j, pos;
  int len;
  int space; /* Flag used to indicate if a space has already been immediately read */
  char* buffer;

  /* We cannot allocate memory for 0 bytes, so
   * we abort the function
   */
  len = strlen(str);
  if (len < 1)
    return;

  buffer = (char*)calloc(len, sizeof(char));
  if (buffer) {
    /* Skip whitespace at the beginning */
    for (i = 0; i < len && isspace(str[i]); i++);

    /* Skip whitespace at the end */
    for (j = len-1; j > i && isspace(str[j]); j--);

    pos = 0; space = 0;
    for (; i <= j; i++) {
      /* If this is the first whitespace we see,
       * add it to the final string and trigger
       * our whitespace-already-seen flag.
       */
      if (isspace(str[i]) && !space) {
        buffer[pos++] = str[i];
        space = 1;
      }
      /* If this is not a whitespace, copy
       * the character to the final string and
       * disable our whitespace-already-seen flag.
       */
      else if (!isspace(str[i])) {
        buffer[pos++] = str[i];
        space = 0;
      }
    }

    /* Replace the original string with the manipulated one. */
    strncpy(str, buffer, pos);
    str[pos] = '\0';

    free(buffer);
  }
}
