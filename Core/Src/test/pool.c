/**
  ******************************************************************************
  * @file           : pool.c
  * @brief          : Memory pool test cases.
  * @date			: July 8th, 2020
  * @author			: Chrisitan M. Schrader
  ******************************************************************************
  */


#include "test/pool.h"

bool memory_pool_tests()
{
	printf("[Memory Pool]\n");
	bool passed = true;

	if (!_test_spawn()) passed = false;

	return passed;
}

bool _test_spawn()
{
	printf("->Pool Spawn:\t");
	struct pool *ptr = pool_spawn(8, 5);
	bool elSize = ptr->elementSize == 8;
	bool elNum = ptr->elementNumber == 5;
	bool used = ptr->usedElements == 0;
	bool next = ptr->nextFree->nextFree->nextFree->nextFree->nextFree == NULL;

	if (elSize && elNum && used && next)
	{
		printf("PASS\n");
		return true;
	}
	else
	{
		printf("FAIL\n");
		return false;
	}
}
