#ifndef __IO_H_
#define __IO_H_

#include "data.h"
#include <stdio.h>

void flushBuffer(void);
int  readData(artwork_info *info);
void readValue(char s[], size_t length);
int  writeData(FILE *file, artwork_info *info);

#endif
