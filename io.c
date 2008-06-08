#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "mem.h"

void
flushBuffer (void)
{
  int c;

  while ((c = getchar ()) != '\n' && c != EOF)
    continue;
}

int
readChar (char *c)
{
  char input[2 + 1];

  /* We read n+1 from the input to be able to check
   * if the user has written exactly n characters */
  readValue (input, 2);

  if (strlen (input) != 1)
    return 1;
  else
    {
      *c = input[0];
      return 0;
    }
}

void
readInt (const char *inputText, char *dest, size_t length)
{
  int i, invalid;

  invalid = 1;

  while (invalid)
    {
      invalid = 0;
      readString (inputText, dest, length);

      for (i = 0; i < strlen (dest); i++)
        {
          if (!isdigit (dest[i]))
            {
              printf ("   Entrada invalida.\n");
              invalid = 1;
              break;
            }
        }
    }
}

void
readString (const char *inputText, char *dest, size_t length)
{
  while (1)
    {
      printf (inputText);
      readValue (dest, length);

      if (strlen (dest) == 0)
        {
          printf ("   Entrada invalida.\n");
          continue;
        }
      else
        break;
    }
}

void
read_word (const char *msg, char *dest, size_t length)
{
  char *space;

  readString (msg, dest, length);

  space = strchr (dest, ' ');
  if (space)
    *space = '\0';
}

void
readValue (char s[], size_t length)
{
  fgets (s, length + 1, stdin);

  stripNewLine (s);
  stripWhiteSpace (s);
}

char *
str_dup (const char *s)
{
  char *dup = malloc (strlen (s) + 1);
  assert (dup);

  strcpy (dup, s);

  return dup;
}

void
str_foreach (char *str, void (*callback) (char *, va_list), ...)
{
  char *next;
  va_list ap;

  va_start (ap, callback);

  /* Get the first word */
  next = strtok (str, " ");
  while (next)
    {
      callback (next, ap);
      next = strtok (NULL, " ");        /* Read next word */
    }

  va_end (ap);
}


void
stripNewLine (char s[])
{
  char *pos = strchr (s, '\n');

  if (!pos)
    flushBuffer ();
  else
    *pos = '\0';
}

void
stripWhiteSpace (char str[])
{
  int i, j, pos;
  int len;
  int space;                    /* Flag used to indicate if a space has already been immediately read */
  char *buffer;

  len = strlen (str);
  if (len < 1)
    return;

  buffer = MEM_ALLOC_N (char, len);

  /* Skip whitespace at the beginning */
  for (i = 0; i < len && isspace (str[i]); i++);

  /* Skip whitespace at the end */
  for (j = len - 1; j > i && isspace (str[j]); j--);

  pos = 0;
  space = 0;
  for (; i <= j; i++)
    {
      /* If this is the first whitespace we see,
       * add it to the final string and trigger
       * our whitespace-already-seen flag.
       */
      if (isspace (str[i]) && !space)
        {
          buffer[pos++] = str[i];
          space = 1;
        }
      /* If this is not a whitespace, copy
       * the character to the final string and
       * disable our whitespace-already-seen flag.
       */
      else if (!isspace (str[i]))
        {
          buffer[pos++] = str[i];
          space = 0;
        }
    }

  /* Replace the original string with the manipulated one. */
  strncpy (str, buffer, pos);
  str[pos] = '\0';

  free (buffer);
}
