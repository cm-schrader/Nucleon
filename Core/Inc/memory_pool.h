/**
  ******************************************************************************
  * @file           : memory_pool.h
  * @brief          : Statically allocate memory pools.
  * @date			: July 8th, 2020
  * @author			: Chrisitan M. Schrader
  ******************************************************************************
  */

#include "cmsis_os.h"

#ifndef INC_MEMORY_POOL_H_
#define INC_MEMORY_POOL_H_

typedef struct freeElement {
	struct freeElement *nextFree;
} freeElement;

typedef struct pool {
	uint32_t elementSize;
	uint32_t elementNumber;
	uint32_t usedElements;
	struct freeElement *nextFree;
} pool;

struct pool* pool_spawn(uint32_t elementSize, uint32_t elementNumber);
//void* pool_malloc(pool);
//void pool_free(pool);
//void pool_free_all(pool);
//void pool_destroy(pool);

#endif /* INC_MEMORY_POOL_H_ */
