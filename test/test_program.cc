#include "catch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

// Go here for more information on using catch for unit testing:
// https://github.com/philsquared/Catch

TEST_CASE("Heap Functions") 
{
    struct heapEntry* freeHeap = initHeap(6); 

    SECTION("Max Heapify Simple")
    {
	freeHeap[1].blockSize = 1;
	freeHeap[2].blockSize = 2;
	freeHeap[3].blockSize = 3;
	freeHeap[4].blockSize = 4;
	setHeapSize(4);
	build_max_heap(freeHeap);

	REQUIRE(freeHeap[1].blockSize == 4);
    }

    SECTION("Max Heapify Complex")
    {
	freeHeap[1].blockSize = 1;
	freeHeap[2].blockSize = 15;
	freeHeap[3].blockSize = 3;
	freeHeap[4].blockSize = 9;
	freeHeap[5].blockSize = 21;
	freeHeap[6].blockSize = 1;
	setHeapSize(6);
	build_max_heap(freeHeap);

	REQUIRE(freeHeap[1].blockSize == 21);
    }

    SECTION("Simple Insert") 
    {
	max_heap_insert(freeHeap, 1, 0);
	max_heap_insert(freeHeap, 3, 0);
	max_heap_insert(freeHeap, 5, 0);
	max_heap_insert(freeHeap, 6, 0);
	max_heap_insert(freeHeap, 9, 0);
	max_heap_insert(freeHeap, 12, 0);
	
        REQUIRE(freeHeap[1].blockSize >= freeHeap[2].blockSize);
	REQUIRE(freeHeap[1].blockSize >= freeHeap[3].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[4].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[5].blockSize);
        REQUIRE(freeHeap[3].blockSize >= freeHeap[6].blockSize);
    }

    SECTION("Complex Insert") 
    {
	max_heap_insert(freeHeap, 28, 4);
	max_heap_insert(freeHeap, 23, 12);
	max_heap_insert(freeHeap, 4, 4);
	max_heap_insert(freeHeap, 16, 4);
	max_heap_insert(freeHeap, 1, 4);
	max_heap_insert(freeHeap, 32, 0);
	
        REQUIRE(freeHeap[1].blockSize >= freeHeap[2].blockSize);
	REQUIRE(freeHeap[1].blockSize >= freeHeap[3].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[4].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[5].blockSize);
        REQUIRE(freeHeap[3].blockSize >= freeHeap[6].blockSize);
    }

    SECTION("Extract Max Test 1")
    {
	struct heapEntry max;
	max_heap_insert(freeHeap, 23, 12);
	max_heap_insert(freeHeap, 28, 4);
	max_heap_insert(freeHeap, 32, 0);
	max = heap_extract_max(freeHeap);

	REQUIRE(max.blockSize == 32);
	REQUIRE(freeHeap[1].blockSize == 28);
    }

    SECTION("Extract Max Test 2")
    {
	struct heapEntry max;
	max_heap_insert(freeHeap, 1, 12);
	max_heap_insert(freeHeap, 2, 4);
	max_heap_insert(freeHeap, 32, 0);
	max_heap_insert(freeHeap, 28, 0);
	max_heap_insert(freeHeap, 28, 0);
	max_heap_insert(freeHeap, 32, 0);
	max = heap_extract_max(freeHeap);
	max = heap_extract_max(freeHeap);
	max = heap_extract_max(freeHeap);
	max = heap_extract_max(freeHeap);

	REQUIRE(max.blockSize == 28);
	REQUIRE(freeHeap[1].blockSize == 2);
    }

    free(freeHeap);
}
