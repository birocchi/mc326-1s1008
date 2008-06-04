#ifndef __HASH_H
#define __HASH_H

#define HASH_FILE_NUM 1 /**< Number of files the memory index is split into */

unsigned int hash_function (char *key);
char *hash_get_filename (const char *prefix, unsigned int hashnum);

#endif
