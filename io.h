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

/*
 * readChar
 *
 * Wrapper around readValue to read a single character (and
 * make sure no more than one character was passed).
 * Returns 0 if everything was OK, and -1 otherwise.
 */
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
void readInt(const char* inputText, char* dest, size_t length);

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
 * This function replaces a trailing '\n' character (if it exists)
 * in a string with '\0'.
 * If a newline character is not found, it flushes the input buffer.
 */
void stripNewLine(char s[]);

/*
 * stripWhiteSpace
 *
 * This function removes trailing whitespaces at the beginning
 * and at the end of the string /str/, and also removes duplicate
 * whitespaces in the middle of the string.
 */
void stripWhiteSpace(char str[]);

#endif
