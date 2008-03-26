#ifndef __DB_H
#define __DB_H

#include <stdio.h>
#include "data.h"

#define PK_REG_SIZE (NAME_LENGTH + RRN_LENGTH)   /* 4 bytes for a 32-bit integer */

typedef struct {
  int rrn;
  char name[NAME_LENGTH];
} primary_key;

int pk_cmpfunc(const void* a, const void* b);
primary_key* createPKFromBase(FILE* base, int* regcount);
int findEntry(primary_key* index, const char* key, int numreg);
primary_key* loadPKFile(FILE* pkfile, int* regcount);
void writePKToFile(primary_key* index, FILE* pkfile, int numreg);

int fileExists(const char* filename);

int getFileSize(FILE * file);
int cmpstring(char **p1, char **p2);
int makeArrayPKIndex(char **pkindex, FILE * base);
int makeFilePKIndex(char ** pkindex, FILE * out, int numreg);
int loadPkFile(char ** pkindex, FILE * pkfile);
char ** search(char **pkindex, char * key, int numreg);

#endif
