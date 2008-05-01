#ifndef __MENU_H_
#define __MENU_H_

#include <stdio.h>

/**
 * \brief Ask the user for the input of an option.
 * \param msg The message to print before receiving the character.
 * \param optstring The valid characters for input.
 * \return The character the user entered.
 *
 * This function asks for user input, which should be a single
 * character present in \a optstring, just like \a getopt(3) works.
 * If an invalid character is entered, the user is asked for input again.
 */
char menuMultipleAnswers (const char *msg, const char *optstring);

/**
 * \brief Ask the user a yes/no question.
 * \param msg The message to print before receiving the character.
 * \retval 0 If the user answered no.
 * \retval 1 If the user answered yes.
 *
 * This is a wrapper around menuMultipleAnswers, passing "sn" as
 * the option string.
 */
int menuYesOrNo (const char *msg);

/**
 * \brief Prin the options for a search in the database.
 */
void printConsultMenu (void);

/**
 * \brief Print the options for the main menu.
 */
void printMainMenu (void);

void print_search_type_menu (void);

/**
 * \brief Print a nifty welcome message.
 */
void printWelcome (void);

#endif
