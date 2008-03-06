#ifndef __IO_H_
#define __IO_H_

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

/* Flushes stdin to remove any leftover characters
 * which may still be lying around */
void flushBuffer(void);

/* Reads the input from the user for one
   artwork only and puts all the obtained data
   into the struct pointed by the parameter pointer.
   Return 1 on error and 0 for OK. */
int readData(artwork_info *info);

/* Writes the data from the struct pointed at by *info
   to the file pointed at by *file.
   Writes it according to the requested parameters.*/
int writeData(FILE *file, artwork_info *info);

#endif
