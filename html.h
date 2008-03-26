#ifndef __HTML_H
#define __HTML_H

#include <stdio.h>
#include "data.h"

void htmlBegin(FILE* htmlfile);
void htmlEnd(FILE* htmlfile);
void htmlWriteRecordInfo(FILE* htmlfile, artwork_info* info);

#endif
