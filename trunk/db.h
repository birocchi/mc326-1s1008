#ifndef __DB_H
#define __DB_H

#include <stdio.h>

int getFileSize(FILE * file);
int cmpstring(char **p1, char **p2);
int makeArrayPKIndex(char **pkindex, FILE * base);
int makeFilePKIndex(char ** pkindex, FILE * out);
int loadPkFile(char ** pkindex, FILE * pkfile);

#endif
