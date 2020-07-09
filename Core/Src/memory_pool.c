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
 * @param  	elementSize:		Size of one element in bytes.
 * @param  	elementNumber: 	Maximum number of elements to allocate space for.
 * @retval 	Pointer to new pool struct.
 *
 * @note 	Because each element needs to be able to store another, the minimum
 * 			elementSize is 8 bytes.
 */
struct pool* pool_spawn(uint32_t elementSize, uint32_t elementNumber)
{
	/* Allocate memory */
	struct pool *ptr = (struct pool *) pvPortMalloc(sizeof(struct pool) + max(elementSize, 8) * elementNumber);
	ptr->elementSize = elementSize;
	ptr->elementNumber  = elementNumber;
	ptr->usedElements = 0;

	/* Traverse to start of pool */
	struct freeElement *el = (struct freeElement *) ptr + sizeof(struct pool) / 4;

	/* Create linked list of free elements */
	for (int i = 0; i < elementNumber - 1; ++i)
	{
		el->nextFree = ++el;
	}
	el->nextFree = NULL;

	return ptr;
}

//pvPortMalloc
//vPortFree
