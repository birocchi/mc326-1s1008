#ifndef __BASE_H
#define __BASE_H

#include <stdio.h>

#define TITLE_LENGTH   200 
#define TYPE_LENGTH    100 
#define AUTHOR_LENGTH  125 
#define YEAR_LENGTH    4   
#define VALUE_LENGTH   12  
#define IMG_LENGTH     9   

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
  char title[TITLE_LENGTH+1]; 
  /** The artwork's type  */
  char type[TYPE_LENGTH+1];
  /** The artworks author */
  char author[AUTHOR_LENGTH+1];
  /** The year the artwork was made */
  char year[YEAR_LENGTH+1];
 /** The artwork's value */
  char value[VALUE_LENGTH+1];      
  /** The register's identifier */
  char img[IMG_LENGTH+1];  
} artwork_info;

void base_read_input(artwork_info* info);

/**
 * Makes the image identifier into an actual file name.
 * Basically it adds the '.' into it.
 * 
 * @param s The name identifier string.
 *
 * @return Pointer to the correct string. One must free this pointer later on.
 *
 */
char* baseGetValidImagePath(const char* s);

/**
 * Checks if the image identifier is valid.
 * This time we use strtol to get past the digits and also
 * check the file extension.
 *
 * @param name The image identifier string.
 * 
 * @return Returns 1 on error and 0 for OK.
 */
int baseIsValidIdentifier(const char* name);

/**
 * Reads one record from the database file /base/ and
 * stores it in /info/.
 *
 * @param base File pointer to the database.
 * @param info Pointer to artwork_info structure with info about the register.
 *
 * @return Returns 0 on succes and 1 on error.
 */
int baseReadArtworkRecord(FILE* base, artwork_info* info);

/**
 * Writes the data from the struct pointed at by *info
 * to the file pointed at by *file.
 * Writes it according to the requested parameters.
 *
 * @param file File pointer to the database.
 * @param info Pointer to artwork_info structure with info about the register.
 */
void base_write_data(FILE *file, artwork_info *info);

#endif
