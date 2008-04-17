#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "file.h"
#include "io.h"
#include "mem.h"
#include "pk.h"
#include "avail.h"

int removedField(FILE * base, int rrn, int * avail){
  int opened;

  if(base){
    opened = 1;
    fclose(base);
    base = fopen(DBFILE, "r+");
  }
  else{
    opened = 0;
    base = fopen(DBFILE, "r+");
  }

  fseek(base, REG_SIZE * rrn, SEEK_SET);
  fprintf(base, "%09d ", *avail);
  *avail = rrn;
  
  fclose(base);

  if(opened){
    base = fopen(DBFILE, "r+");
  }

  return 0;

}
