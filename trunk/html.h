#ifndef __HTML_H
#define __HTML_H

#include <stdio.h>
#include "base.h"

/**
 * \brief Writes the beginning of an HTML file to \a htmlfile.
 * \param htmlfile The file pointer to the HTML file.
 */
void html_begin (FILE * htmlfile);

/**
 * \brief Writes the end of an HTML file to \a htmlfile.
 * \param htmlfile The file pointer to the HTML file.
 */
void html_end (FILE * htmlfile);

/**
 * \brief Writes the information for a single record to \a htmlfile.
 * \param htmlfile The file pointer to the HTML file.
 * \param info The structure that holds all the info fields for the register.
 */
void html_write_record_info (FILE * htmlfile, ArtworkInfo * info);

#endif
