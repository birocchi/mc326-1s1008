#ifndef __HTML_H
#define __HTML_H

#include <stdio.h>
#include "base.h"

/**
 * Writes the beginning of an HTML file to /htmlfile/.
 *
 * @param htmlfile The file pointer to the HTML file.
 *
 * @retval Returns nothing.
 */
void htmlBegin(FILE* htmlfile);

/**
 * Writes the end of an HTML file to /htmlfile/.
 *
 * @param htmlfile The file pointer to the HTML file.
 *
 * @retval Returns nothing.
 */
void htmlEnd(FILE* htmlfile);

/**
 * Writes the information for a single record to /htmlfile/.
 *
 * @param htmlfile The file pointer to the HTML file.
 * @param info The structure that holds all the info fields for the register.
 *
 * @retval Returns nothing.
 */
void htmlWriteRecordInfo(FILE* htmlfile, ArtworkInfo* info);

#endif
