#ifndef __HTML_H
#define __HTML_H

#include <stdio.h>
#include "base.h"

/**
 * \brief Writes the beginning of an HTML file to \a htmlfile.
 * \param htmlfile The file pointer to the HTML file.
 */
void htmlBegin(FILE *htmlfile);

/**
 * \brief Writes the end of an HTML file to \a htmlfile.
 * \param htmlfile The file pointer to the HTML file.
 */
void htmlEnd(FILE *htmlfile);

/**
 * \brief Writes the information for a single record to \a htmlfile.
 * \param htmlfile The file pointer to the HTML file.
 * \param info The structure that holds all the info fields for the register.
 */
void htmlWriteRecordInfo(FILE *htmlfile, ArtworkInfo *info);

#endif
