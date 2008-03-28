#ifndef __MEM_H
#define __MEM_H

#include <stdlib.h>

/*
 * MEM_ALLOC
 *
 * Wrapper over MEM_ALLOC_N. Equivalent to allocating
 * memory for 1 entry of type /type/.
 */
#define MEM_ALLOC(type) (MEM_ALLOC_N(type, 1))

/*
 * MEM_ALLOC_N
 *
 * Allocates memory for /numelem/ elements of type /type/.
 */
#define MEM_ALLOC_N(type, numelem) ((type*)__memAllocate(numelem, sizeof(type)))

/*
 * __memAllocate
 *
 * This function should not be called directly. It's a wrapper
 * over calloc.
 * If memory can't be properly allocated, it displays an error
 * message and exits the program.
 */
void* __memAllocate(size_t numelem, size_t elemsize);

#endif
