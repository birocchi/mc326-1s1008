#ifndef __IO_H
#define __IO_H

#include <stdarg.h>
#include <stdio.h>

/**
 * \brief Wrapper around read_value to read a single character (and
 *        make sure no more than one character was passed).
 *
 *        Removes trailing spaces before and after the text.
 *
 * \param c Pointer to store the character read.
 *
 * \retval -1 Error.
 * \retval 0 Success.
 */
int read_char (char *c);

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
void read_int (const char *inputText, char *dest, size_t length);

/**
 * \brief Read input from the user until it is not null.
 *
 *        Removes all trailing whitespace before checking if the text is null.
 *
 * \param inputText The text to print before waiting for input.
 * \param dest      Pointer to the place where we want to store the input.
 * \param length    The maximum number of characters that can be stored.
 */
void read_string (const char *inputText, char *dest, size_t length);

/**
 * @brief Read a single word from the user.
 *
 * @param msg     Message to print before waiting for input.
 * @param dest    Pointer to the place where the input should be stored.
 * @param length  Maximum number of characters that can be stored.
 *
 * This function reads some string from the keyboard, and stores to \a dest
 * the first word (considering that the string is space-delimited).
 */
void read_word (const char *msg, char *dest, size_t length);

/**
 * \brief Read at most length bytes from stdin. Strip trailing whitespace and
 *        newline character if they're present.
 *
 *        If no newline is found, clear the input buffer before leaving.
 *
 * \param s       Where to store the input.
 * \param length  The maximum number of characters that can be stored.
 */
void read_value (char s[], size_t length);

/**
 * @brief strdup implementation.
 *
 * @param  s String to duplicate.
 *
 * @return Pointer to a newly allocated copy of \a s.
 *
 * This is a small strdup implementation, as strdup itself is not
 * part of ISO C.
 */
char *str_dup (const char *s);

/**
 * @brief Runs \a callback on each word of a string.
 *
 * @param str       The string which will be parsed.
 * @param callback  The function which will be called.
 * @param ...       Additional parameters for \a callback.
 *
 * This function calls \a callback on each word (sequence of characters
 * delimited by a space) of \a str.
 * \a callback must be a function which takes the current word and a \a va_list
 * as parameters.
 * \b WARNING: \a str is going to be changed because this function relies on
 * \a strtok.
 */
void str_foreach (char *str, void (*callback) (char *, va_list), ...);

/**
 * @brief Returns the concatenation of \a a and \a b.
 *
 * @param a The first string.
 * @param b The second string.
 *
 * Allocates memory for a new char* that is \a b concatenated to \a a
 * and return it.
 */
char *str_join (const char *a, const char *b);

/**
 * \brief Replaces the trailing '\\n' character (if it exists) in a string
 *        with '\\0'.
 *
 *        If a newline character is not found, it flushes the input buffer.
 *
 * \param s The string which should be stripped.
 */
void stripNewLine (char s[]);

/**
 * \brief Removes whitespace from the beginning and the end of the string,
 *        and also duplicate whitespace in the middle of the text.
 *
 * \param str The string which should be stripped.
 */
void stripWhiteSpace (char str[]);

#endif
