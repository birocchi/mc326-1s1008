#ifndef __HTML_H
#define __HTML_H

#include <stdio.h>
#include "base.h"

/*
 * htmlBegin
 *
 * Writes the beginning of an HTML file to /htmlfile/.
 */
void htmlBegin(FILE* htmlfile);

/*
 * htmlEnd
 *
 * Writes the end of an HTML file to /htmlfile/.
 */
void htmlEnd(FILE* htmlfile);

/*
 * htmlWriteRecordInfo
 *
 * Writes the information for a single record to /htmlfile/.
 */
void htmlWriteRecordInfo(FILE* htmlfile, artwork_info* info);

#endif
