/**
  ******************************************************************************
  * @file           : memory_pool.c
  * @brief          : Statically allocate memory pools.
  * @date			: July 8th, 2020
  * @author			: Chrisitan M. Schrader
  ******************************************************************************
  */

#include "memory_pool.h"

/**
 * @brief Creates a memory pool.
 * @param  	elementSize:	Size of one element in bytes. Must be 8 or greater.
 * @param  	elementNumber: 	Maximum number of elements to allocate space for.
 * @retval 	Pointer to new pool struct or NULL pointer.
 *
 * @note 	Because each element needs to be able to a pointer to another element,
 * 			the minimum elementSize is 8 bytes.
 */
struct pool* pool_spawn(uint32_t elementSize, uint32_t elementNumber)
{
	if (elementSize < 8) return NULL;

	/* Allocate memory */
	struct pool *ptr = (struct pool *) pvPortMalloc(sizeof(struct pool) + elementSize * elementNumber);
	ptr->elementSize = elementSize;
	ptr->elementNumber  = elementNumber;
	ptr->usedElements = 0;

	/* Traverse to start of pool */
	struct freeElement *el = (struct freeElement *) ptr + sizeof(struct pool) / 4;
	struct freeElement *nextEl;
	ptr->nextFree = el;

	/* Create linked list of free elements */
	for (int i = 0; i < elementNumber - 1; ++i)
	{
		nextEl = el + elementSize / 4;
		el->nextFree = nextEl;
		el = nextEl;
	}
	el->nextFree = NULL;

	return ptr;
}

//pvPortMalloc
//vPortFree
