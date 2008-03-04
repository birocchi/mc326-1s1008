#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

/*
 * stripNewLine
 * This function looks for a trailing '\n' character in a string,
 * replaces it with a '\0' and returns 1.
 * If there is no trailing newline, returns 0.
 */
static int stripNewLine(char s[])
{
	int pos;

	for (pos = 0; pos < strlen(s); pos++) {
		if (pos[i] == '\n') {
			pos[i] = '\0';
			return 1;
		}
	}

	return 0;
}

/*
 * readValue
 * Reads at most length bytes from stdin and
 * strips the trailing newline if it is present.
 * Otherwise, clear the input buffer before leaving.
 */
static void readValue(char s[], size_t length)
{
	int c;

	/* fgets reads n-1 characters from the stream, so we
	 * use length+1 to make readValue calls keep making sense. */
	fgets(s, length+1, stdin);

	if (!stripNewLine(s)) {
		while ((c = getchar()) != '\n' && c != EOF)
			continue;
	}
}

int readData(artwork_info *info)
{
	char value[10];
	char year[5];

	if (!info) {
		return 1;
	} /* Returns an error if the pointer is NULL. */

	printf("Por favor, digite o titulo da obra: ");
	readValue(info->title, 200);

	printf("\nPor favor, digite o tipo da obra: ");
	readValue(info->type, 100);

	printf("\nPor favor, digite o autor da obra: ");
	readValue(info->author, 100);

	printf("\nPor favor, digite o ano da obra: ");
	readValue(year, 4);
	info->year = atoi(year);

	printf("\nPor favor, digite o valor da obra: ");
	readValue(value, 9);
	info->value = atoi(value);

	printf("\nPor favor, digite o identificador da obra: ");
	readValue(info->img, 7);

	return 0;
}

int writeData(FILE *file, artwork_info *info)
{
	/* Return error if the file or struct pointers are NULL. */
	if (!file | !info) {
		return 1;
	}

	fprintf(file, "%-200s",   info->title);
	fprintf(file, "%-100s",   info->type);
	fprintf(file, "%-100s",   info->author);
	fprintf(file, "%04d",     info->year);
	fprintf(file, "%09d",     info->value);
	fprintf(file, "%-7s",     info->img);

	return 0;
}
