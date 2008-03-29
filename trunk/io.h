#ifndef __IO_H_
#define __IO_H_

#include "base.h"
#include <stdio.h>

/* 
 * flushBuffer
 *
 * Flushes stdin to remove any leftover characters
 * which may still be lying around
 */
void flushBuffer(void);

int readChar(char* c);
/* 
 * readData
 *
 * Reads the input from the user for one
 * artwork only and puts all the obtained data
 * into the struct pointed by the parameter pointer.
 * Return 1 on error and 0 for OK.
 */
int readData(artwork_info *info);

/*
 * readInt
 *
 * Reads input and checks if it's a valid integer. If not,
 * read it until a valid integer is entered.
 */
void readInt(const char* inputText, int* dest, size_t length);

/*
 * readString
 *
 * Read input without much validation. If it is a null entry,
 * read it again.
 */
void readString(const char* inputText, char* dest, size_t length);

/*
 * readValue
 *
 * Reads at most length bytes from stdin and
 * strips the trailing newline if it is present.
 * Otherwise, clear the input buffer before leaving.
 */
void readValue(char s[], size_t length);

/*
 * stripNewLine
 *
 * This function looks for a trailing '\n' character in a string,
 * replaces it with a '\0' and returns 1.
 * If there is no trailing newline, returns 0.
 */
int stripNewLine(char s[]);


#endif
