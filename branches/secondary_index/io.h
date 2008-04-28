#ifndef __IO_H_
#define __IO_H_

#include "base.h"
#include <stdio.h>

/**
 * \brief The group number used in the image identifier.
 */
#define GROUP_NUMBER "01" 

/**
 * \brief Flushes stdin to remove any leftover characters which
 *        may still be lying around.
 */
void flushBuffer(void);

/**
 * \brief Wrapper around readValue to read a single character (and
 *        make sure no more than one character was passed).
 *
 *        Removes trailing spaces before and after the text.
 *
 * \param c Pointer to store the character read.
 *
 * \retval -1 Error.
 * \retval 0 Success.
 */
int readChar(char* c);

/**
 * \brief Reads input and checks if it's a valid integer. If not,
 *        read it until a valid integer is entered.
 *
 *        Removes all trailing spaces before checking if the text is valid.
 *
 * \param inputText The text to print before waiting for input.
 * \param dest      Pointer to the place where we want to store the input.
 * \param length    The maximum number of characters that can be stored.
 */
void readInt(const char* inputText, char* dest, size_t length);

/**
 * \brief Read input from the user until it is not null.
 *
 *        Removes all trailing whitespace before checking if the text is null.
 *
 * \param inputText The text to print before waiting for input.
 * \param dest      Pointer to the place where we want to store the input.
 * \param length    The maximum number of characters that can be stored.
 */
void readString(const char* inputText, char* dest, size_t length);

/**
 * \brief Read at most length bytes frm stdin. Strip trailing whitespace and
 *        newline character if they're present.
 *
 *        If no newline is found, clear the input buffer before leaving.
 *
 * \param s       Where to store the input.
 * \param length  The maximum number of characters that can be stored.
 */
void readValue(char s[], size_t length);

char* str_dup (const char *s);

/**
 * \brief Replaces the trailing '\\n' character (if it exists) in a string
 *        with '\\0'.
 *
 *        If a newline character is not found, it flushes the input buffer.
 *
 * \param s The string which should be stripped.
 */
void stripNewLine(char s[]);

/**
 * \brief Removes whitespace from the beginning and the end of the string,
 *        and also duplicate whitespace in the middle of the text.
 *
 * \param str The string which should be stripped.
 */
void stripWhiteSpace(char str[]);

#endif
