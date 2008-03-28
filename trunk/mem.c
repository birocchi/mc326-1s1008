#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

void* __memAllocate(size_t numelem, size_t elemsize) {
  void* chunk = calloc(numelem, elemsize);

  if (chunk == NULL) {
    fprintf(stderr, "Erro ao alocar %d bytes. Abortando programa.\n", numelem*elemsize);
    exit(EXIT_FAILURE);
  }

  return chunk;
}
