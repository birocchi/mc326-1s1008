#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

void *
__memAllocate (size_t numelem, size_t elemsize)
{
  void *chunk = calloc (numelem, elemsize);     /* Allocates a new array. */

  assert (chunk != NULL);

  return chunk;
}
