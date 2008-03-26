#ifndef __DB_H
#define __DB_H

#include <stdio.h>

#define PK_REGSIZE 210 /* The size of a primary key record. */

int getFileSize(FILE * file);
int cmpstring(char **p1, char **p2);
int makeArrayPKIndex(char **pkindex, FILE * base);
int makeFilePKIndex(char ** pkindex, FILE * out, int numreg);
int loadPkFile(char ** pkindex, FILE * pkfile);
char ** search(char **pkindex, char * key, int numreg);

#endif
