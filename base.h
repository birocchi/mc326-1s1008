#ifndef __BASE_H
#define __BASE_H

#include <stdio.h>
#include "avail.h"

#define TITLE_LENGTH   200 /**< Length of the title field. */
#define TYPE_LENGTH    100 /**< Length of the type field. */
#define AUTHOR_LENGTH  125 /**< Length of the author field. */
#define YEAR_LENGTH    4   /**< Length of the year field. */
#define VALUE_LENGTH   12  /**< Length of the value field. */
#define IMG_LENGTH     9   /**< Length of the image identifier field. */

/**
 * Total size of the register.
 */
#define BASE_REG_SIZE (TITLE_LENGTH + TYPE_LENGTH + AUTHOR_LENGTH + \
                       YEAR_LENGTH + VALUE_LENGTH + IMG_LENGTH)

/**
 * The main structure which holds data about
 * each register in the database
 * We use FIELD_LENGTH+1 because we need one more
 * byte to hold the '\\0' when using fgets.
 */
typedef struct
{
  /** The artwork's title  */
  char title[TITLE_LENGTH + 1];
  /** The artwork's type  */
  char type[TYPE_LENGTH + 1];
  /** The artworks author */
  char author[AUTHOR_LENGTH + 1];
  /** The year the artwork was made */
  char year[YEAR_LENGTH + 1];
 /** The artwork's value */
  char value[VALUE_LENGTH + 1];
  /** The register's identifier */
  char img[IMG_LENGTH + 1];
} ArtworkInfo;

/**
 * Structure which represents the base file and
 * its avail list.
 */
typedef struct
{
  AvailList *avlist;  /**< The base's avail list. */
  FILE *fp;           /**< The base's file representation. */
} Base;

/**
 * \brief Frees all the base allocated memory.
 *        
 * \param b Pointer to Base struct.
 */
void base_free (Base * b);

/**
 * \brief Inserts a new artwork to the database.
 *        Considers the avail list already.
 *
 * \param base Pointer to Base struct.
 * \param info Pointer to ArtworkInfo struct, where the new info is.
 * @return The RRN of the new entry.
 */
int base_insert (Base * base, ArtworkInfo * info);

/**
 * \brief Creates a new database struct.
 *        Loads it from file if it exists.
 *
 * \param basename Pointer to const char that is the name of the database file.
 * \param availname Pointer to const char that is the name of avail list file.
 *
 * \return A new Base structure.
 */
Base *base_new (const char *basename, const char *availname);

/**
 * \brief Removes the one register at position \a rrn.
 *
 * \param base File pointer to the database.
 * \param rrn Integer that corresponds to the position in the database.
 */
void base_remove (Base * base, int rrn);

/**
 * \brief Reads all the input fields for the artwork and saves it
 *        into \a info.
 *
 * \param info Pointer to ArtworkInfo structure with info about the register.
 */
void base_read_input (ArtworkInfo * info);

/**
 * Makes the image identifier into an actual file name.
 * Basically it adds the '.' into it.
 * 
 * @param s The name identifier string.
 *
 * @return Pointer to the correct string. One must free this pointer later on.
 */
char *baseGetValidImagePath (const char *s);

/**
 * Checks if the image identifier is valid.
 * This time we use strtol to get past the digits and also
 * check the file extension.
 *
 * @param name The image identifier string.
 * 
 * @return Returns 1 on error and 0 for OK.
 */
int baseIsValidIdentifier (const char *name);

/**
 * \brief Reads one record from the database file \a base and
 *        stores it in \a info.
 *
 * \param base File pointer to the database.
 * \param info Pointer to ArtworkInfo structure with info about the register.
 */
void base_read_artwork_record (FILE * base, ArtworkInfo * info);

/**
 * Writes the data from the struct pointed at by *info
 * to the file pointed at by *file.
 * Writes it according to the requested parameters.
 *
 * @param file File pointer to the database.
 * @param info Pointer to ArtworkInfo structure with info about the register.
 */
void base_write_data (FILE * file, ArtworkInfo * info);

#endif
