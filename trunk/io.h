#include "data.h"
#include <stdio.h>
#include <stdlib.h>

/* Reads the input from the user for one
   artwork only and puts all the obtained data
   into the struct pointed by the parameter pointer.
   Return 1 on error and 0 for OK. */
int read_data(obra_info *info);


/* Writes the data from the struct pointed at by *info
   to the file pointed at by *file. 
   Writes it according to the requested parameters.*/
int write_data(FILE *file, obra_info *info);
