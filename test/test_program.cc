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
	buildMaxHeap(freeHeap);

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
	buildMaxHeap(freeHeap);

	REQUIRE(freeHeap[1].blockSize == 21);
    }

    SECTION("Simple Insert") 
    {
	maxHeapInsert(freeHeap, 1, 0);
	maxHeapInsert(freeHeap, 3, 0);
	maxHeapInsert(freeHeap, 5, 0);
	maxHeapInsert(freeHeap, 6, 0);
	maxHeapInsert(freeHeap, 9, 0);
	maxHeapInsert(freeHeap, 12, 0);
	
        REQUIRE(freeHeap[1].blockSize >= freeHeap[2].blockSize);
	REQUIRE(freeHeap[1].blockSize >= freeHeap[3].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[4].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[5].blockSize);
        REQUIRE(freeHeap[3].blockSize >= freeHeap[6].blockSize);
    }

    SECTION("Complex Insert") 
    {
	maxHeapInsert(freeHeap, 28, 4);
	maxHeapInsert(freeHeap, 23, 12);
	maxHeapInsert(freeHeap, 4, 4);
	maxHeapInsert(freeHeap, 16, 4);
	maxHeapInsert(freeHeap, 1, 4);
	maxHeapInsert(freeHeap, 32, 0);
	
        REQUIRE(freeHeap[1].blockSize >= freeHeap[2].blockSize);
	REQUIRE(freeHeap[1].blockSize >= freeHeap[3].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[4].blockSize);
        REQUIRE(freeHeap[2].blockSize >= freeHeap[5].blockSize);
        REQUIRE(freeHeap[3].blockSize >= freeHeap[6].blockSize);
    }

    SECTION("Extract Max Test 1")
    {
	struct heapEntry max;
	maxHeapInsert(freeHeap, 23, 12);
	maxHeapInsert(freeHeap, 28, 4);
	maxHeapInsert(freeHeap, 32, 0);
	max = heapExtractMax(freeHeap);

	REQUIRE(max.blockSize == 32);
	REQUIRE(freeHeap[1].blockSize == 28);
    }

    SECTION("Extract Max Test 2")
    {
	struct heapEntry max;
	maxHeapInsert(freeHeap, 1, 12);
	maxHeapInsert(freeHeap, 2, 4);
	maxHeapInsert(freeHeap, 32, 0);
	maxHeapInsert(freeHeap, 28, 0);
	maxHeapInsert(freeHeap, 28, 0);
	maxHeapInsert(freeHeap, 32, 0);
	max = heapExtractMax(freeHeap);
	max = heapExtractMax(freeHeap);
	max = heapExtractMax(freeHeap);
	max = heapExtractMax(freeHeap);

	REQUIRE(max.blockSize == 28);
	REQUIRE(freeHeap[1].blockSize == 2);
    }

    free(freeHeap);
}

TEST_CASE("Hash Functions") 
{
	struct symbolTableEntry* symTable = (struct symbolTableEntry*) malloc(11 * sizeof(symbolTableEntry));
	for(int i = 0; i < 11; i++)
		symTable[i].type = -1;
	char varName[SYMBOL_LENGTH] = "Sky";

	SECTION("Hash Function")
	{
		strcpy(varName, "Sky");
		int key = hashFunc(varName, 11);	

		REQUIRE(key == 3);
	}

	SECTION("Insert single")
	{
		setNumEntries(0);
		strcpy(varName, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			

		REQUIRE(strcmp("Sky", symTable[3].symbol) == 0);
	}

	SECTION("Insert multiple")
	{
		setNumEntries(0);
		strcpy(varName, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "Balloon");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "27");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			

		REQUIRE(strcmp("Sky", symTable[3].symbol) == 0);
		REQUIRE(strcmp("Balloon", symTable[7].symbol) == 0);
		REQUIRE(strcmp("27", symTable[6].symbol) == 0);
	}

	SECTION("Single Collision")
	{
		setNumEntries(0);
		strcpy(varName, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "Zz");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
	
		REQUIRE(strcmp("Sky", symTable[3].symbol) == 0);
		REQUIRE(strcmp("Zz", symTable[4].symbol) == 0);
	}

	SECTION("Multiple Collisions")
	{
		setNumEntries(0);
		strcpy(varName, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "Zz");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "Balloon");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "T");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
	
		REQUIRE(strcmp("Sky", symTable[3].symbol) == 0);
		REQUIRE(strcmp("Zz", symTable[4].symbol) == 0);
		REQUIRE(strcmp("Balloon", symTable[7].symbol) == 0);
		REQUIRE(strcmp("T", symTable[8].symbol) == 0);
	}

	SECTION("Looping Collision")
	{
		setNumEntries(0);
		strcpy(varName, "67");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			
		strcpy(varName, "76");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			
	
		REQUIRE(strcmp("67", symTable[10].symbol) == 0);
		REQUIRE(strcmp("76", symTable[0].symbol) == 0);
	}

	SECTION("Remove Single")
	{
		setNumEntries(0);
		strcpy(varName, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			

		hashTableRemove(symTable, 11, "Sky");
		
		REQUIRE(symTable[3].type == -2);
	}

	SECTION("Remove Multiple")
	{
		setNumEntries(0);
		strcpy(varName, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "Balloon");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		strcpy(varName, "27");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			

		hashTableRemove(symTable, 11, "Sky");
		hashTableRemove(symTable, 11, "Balloon");
		hashTableRemove(symTable, 11, "27");
	
		REQUIRE(symTable[3].type == -2);
		REQUIRE(symTable[7].type == -2);
		REQUIRE(symTable[6].type == -2);
	}

	SECTION("Remove then Insert")
	{
		setNumEntries(0);
		strcpy(varName, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
		hashTableRemove(symTable, 11, "Sky");
		hashTableInsert(symTable, 11, varName, CHAR, 0, 3);			
	
		REQUIRE(strcmp("Sky", symTable[3].symbol) == 0);
	}

	SECTION("Remove after Collision")
	{
		setNumEntries(0);
		strcpy(varName, "67");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			
		strcpy(varName, "76");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			
		strcpy(varName, "M");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			

		hashTableRemove(symTable, 11, "M");

		REQUIRE(symTable[1].type == -2);
	}

	SECTION("Basic Search")
	{
		setNumEntries(0);
		strcpy(varName, "67");
		hashTableInsert(symTable, 11, varName, INT, 0, 3);			

		int index = hashTableSearch(symTable, 11, varName);

		REQUIRE(index == 10);
	}
}
