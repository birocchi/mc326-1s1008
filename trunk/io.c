#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

/*
 * readInt
 *
 * Reads input and checks if it's a valid integer. If not,
 * read it until a valid integer is entered.
 */
static void readInt(const char* inputText, int* dest, size_t length)
{
  char* endptr;
  char tmp[length+1];

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

/*
 * readString
 *
 * Read input without much validation. If it is a null entry,
 * read it again.
 */
static void readString(const char* inputText, char* dest, size_t length)
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

/*
 * stripNewLine
 *
 * This function looks for a trailing '\n' character in a string,
 * replaces it with a '\0' and returns 1.
 * If there is no trailing newline, returns 0.
 */
static int stripNewLine(char s[]) {
  int pos;

  for (pos = 0; pos < strlen(s); pos++) {
    if (s[pos] == '\n') {
      s[pos] = '\0';
      return 1;
    }
  }

  return 0;
}

/*
 * validateIdentifier
 *
 * Checks if the image identifier is valid.
 * This time we use strtol to get past the digits and also
 * check the file extension.
 *
 * Returns 1 on error and 0 for OK.
 */
static int validateIdentifier(const char* name)
{
  char* endptr;
  int i;

  i = strtol(name, &endptr, 10);
  if ((endptr == '\0') || strlen(endptr) != 3 || ((strncmp(endptr, "jpg", 3)) &&
        strncmp(endptr, "gif", 3) && (strncmp(endptr, "png", 3))))
    return 1;
  else
    return 0;
}

/* 
 * flushBuffer
 *
 * Flushes stdin to remove any leftover characters
 * which may still be lying around
 */
void flushBuffer(void) {
  int c;

  while ((c = getchar()) != '\n' && c != EOF)
    continue;
}

/* 
 * readData
 *
 * Reads the input from the user for one
 * artwork only and puts all the obtained data
 * into the struct pointed by the parameter pointer.
 * Return 1 on error and 0 for OK.
 */
int readData(artwork_info *info) {
  char img[10];

  /* Returns an error if the pointer is NULL. */
  if (!info)
    return 1;

  readString("\n   Por favor, digite o titulo da obra: ", info->title, 200);
  readString("\n   Por favor, digite o tipo da obra: ", info->type, 100);
  readString("\n   Por favor, digite o autor da obra: ", info->author, 125);

  readInt("\n   Por favor, digite o ano da obra: ", &(info->year), 4);
  readInt("\n   Por favor, digite o valor da obra: ", &(info->value), 12);

  while (1) {
    readString("\n   Por favor, digite o identificador da obra: ", img, 9);

    /* Validate the image identifier */
    if (!validateIdentifier(img)) {
      strncpy(info->img, img, 10);
      break;
    }
    else {
      printf("   Entrada invalida.");
      continue;
    }
  }

  return 0;
}

/*
 * readValue
 *
 * Reads at most length bytes from stdin and
 * strips the trailing newline if it is present.
 * Otherwise, clear the input buffer before leaving.
 */
void readValue(char s[], size_t length) {
  /* fgets reads n-1 characters from the stream, so we
   * use length+1 to make readValue calls keep making sense. */
  fgets(s, length+1, stdin);

  if (!stripNewLine(s))
    flushBuffer();
}

/*
 * writeData
 *
 * Writes the data from the struct pointed at by *info
 * to the file pointed at by *file.
 * Writes it according to the requested parameters.
 */
int writeData(FILE *file, artwork_info *info) {
  /* Return error if the file or struct pointers are NULL. */
  if (!file || !info) {
    return 1;
  }

  fprintf(file, "%-200s",   info->title);
  fprintf(file, "%-100s",   info->type);
  fprintf(file, "%-125s",   info->author);
  fprintf(file, "%04d",     info->year);
  fprintf(file, "%012d",     info->value);
  fprintf(file, "%s",     info->img);

  return 0;
}

/*
 * makeHtml
 *
 * Generates a html format file for browser viewing.
 * FILE *base is our database.
 * FILE *html is the html destination file pointer.
 */

int makeHtml(FILE *base, FILE *html){

  if (!base || !html){
    return 1;
  }


  fprintf(html, "<html>\n<head>\n</head><body>\n");
  fprintf(html, "<p>teste\n");

  

  /* get base's size to know how many entries..
     then read that many entries part by part
     fprinting its data adequately.*/
  

  fprintf(html, "</body>\n</html>\n");


  return 0;
}
