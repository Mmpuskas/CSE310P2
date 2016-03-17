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
	free(symTable);
}

TEST_CASE("Control Path Tests")
{
	int prime = 11;
	int memSize = pow(2,5);
	int heapSize = 8;
	char* mem = initMem(memSize);	
    	struct heapEntry* freeHeap = initHeap(heapSize); 
	maxHeapInsert(freeHeap, memSize, 0);

	struct symbolTableEntry* symTable = (struct symbolTableEntry*) malloc(prime * sizeof(symbolTableEntry));
	for(int i = 0; i < prime; i++)
		symTable[i].type = -1;

	SECTION("Init Memory")
	{
		REQUIRE(mem);
	}

	SECTION("Allocate Small INT")
	{
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);

		REQUIRE(symTable[hashTableSearch(symTable, prime, "a")].offset == 0);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "a")].noBytes == 4);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "a")].type == INT);
		REQUIRE((unsigned int) mem[0] == 3);
		REQUIRE((unsigned int) mem[1] == 0);
		REQUIRE(heapExtractMax(freeHeap).blockSize == 28);
	}
	
	SECTION("Allocate Large INT")
	{
		myMallocInt(mem, symTable, freeHeap, prime, "a", 12345);

		REQUIRE((unsigned int) mem[0] == 57);
		REQUIRE((unsigned int) mem[1] == 48);
		REQUIRE(heapExtractMax(freeHeap).blockSize == 28);
	}

	SECTION("Allocate TWO INTs (Oh dang)")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);
		myMallocInt(mem, symTable, freeHeap, prime, "b", 7);

		REQUIRE(symTable[hashTableSearch(symTable, prime, "a")].offset == 0);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "a")].noBytes == 4);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "a")].type == INT);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "b")].offset == 4);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "b")].noBytes == 4);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "b")].type == INT);
		REQUIRE((unsigned int) mem[0] == 3);
		REQUIRE((unsigned int) mem[4] == 7);
	}

	SECTION("Allocate CHAR")
	{
		setNumEntries(0);
		myMallocChar(mem, symTable, freeHeap, prime, "s", 2, "h");

		REQUIRE(symTable[hashTableSearch(symTable, prime, "s")].offset == 0);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "s")].noBytes == 2);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "s")].type == CHAR);
		REQUIRE(strcmp(((char *) &mem[0]), "h") == 0);	
	}

	SECTION("Allocate 2 CHARs")
	{
		setNumEntries(0);
		myMallocChar(mem, symTable, freeHeap, prime, "s", 2, "h");
		myMallocChar(mem, symTable, freeHeap, prime, "p", 4, "hey");

		REQUIRE(strcmp(((char *) &mem[0]), "h") == 0);	
		REQUIRE(strcmp(((char *) &mem[4]), "hey") == 0);	
		REQUIRE(symTable[hashTableSearch(symTable, prime, "s")].offset == 0);
		REQUIRE(symTable[hashTableSearch(symTable, prime, "p")].offset == 4);
	}

	SECTION("Allocate mix of CHAR and INT")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 12345);
		myMallocChar(mem, symTable, freeHeap, prime, "s", 2, "h");
		myMallocChar(mem, symTable, freeHeap, prime, "p", 4, "hey");
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);
	
		REQUIRE((unsigned int) mem[0] == 57);
		REQUIRE((unsigned int) mem[1] == 48);
		REQUIRE(strcmp(((char *) &mem[4]), "h") == 0);
		REQUIRE(strcmp(((char *) &mem[8]), "hey") == 0);	
		REQUIRE((unsigned int) mem[12] == 3);
	}

	SECTION("Free Single")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 12345);
		myMallocInt(mem, symTable, freeHeap, prime, "b", 12345); //To avoid empty err
		REQUIRE(hashTableSearch(symTable, prime, "a") == 9);
		
		myFree(mem, symTable, freeHeap, prime, "a");
		REQUIRE(hashTableSearch(symTable, prime, "a") == -1);
	}

	SECTION("Add with Scalar")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);
		REQUIRE((unsigned int) mem[0] == 3);

		myAdd(mem, symTable, prime, "a", "3");	
		REQUIRE((unsigned int) mem[0] == 6);
	}

	SECTION("Add with 2 Vars")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);
		REQUIRE((unsigned int) mem[0] == 3);
		myMallocInt(mem, symTable, freeHeap, prime, "b", 4);
		REQUIRE((unsigned int) mem[4] == 4);

		myAdd(mem, symTable, prime, "a", "b");	
		REQUIRE((unsigned int) mem[0] == 7);
	}

	SECTION("Compact 2")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);
		myMallocInt(mem, symTable, freeHeap, prime, "b", 4);
		myFree(mem, symTable, freeHeap, prime, "a");
		myFree(mem, symTable, freeHeap, prime, "b");
		
		myCompact(freeHeap);

		struct heapEntry max;
		max = heapExtractMax(freeHeap);
		REQUIRE(max.blockSize == 32);
	}

	SECTION("Compact 2 Complex")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);
		myMallocInt(mem, symTable, freeHeap, prime, "b", 4);
		myMallocInt(mem, symTable, freeHeap, prime, "c", 5);
		myMallocInt(mem, symTable, freeHeap, prime, "d", 6);
		myFree(mem, symTable, freeHeap, prime, "a");
		myFree(mem, symTable, freeHeap, prime, "b");
		
		myCompact(freeHeap);

		struct heapEntry max;
		max = heapExtractMax(freeHeap);
		REQUIRE(max.blockSize == 16);
		max = heapExtractMax(freeHeap);
		REQUIRE(max.blockSize == 8);
	}

	SECTION("Compact Multiple")
	{
		setNumEntries(0);
		myMallocInt(mem, symTable, freeHeap, prime, "a", 3);
		myMallocInt(mem, symTable, freeHeap, prime, "b", 4);
		myMallocInt(mem, symTable, freeHeap, prime, "c", 5);
		myMallocInt(mem, symTable, freeHeap, prime, "d", 6);
		myMallocInt(mem, symTable, freeHeap, prime, "e", 7);
		myMallocInt(mem, symTable, freeHeap, prime, "f", 8);
		myFree(mem, symTable, freeHeap, prime, "b");
		myFree(mem, symTable, freeHeap, prime, "c");
		myFree(mem, symTable, freeHeap, prime, "e");
		myFree(mem, symTable, freeHeap, prime, "f");

		struct heapEntry max;

		myCompact(freeHeap);

		max = heapExtractMax(freeHeap);
		REQUIRE(max.blockSize == 16);
		max = heapExtractMax(freeHeap);
		REQUIRE(max.blockSize == 8);
	}

	SECTION("String Concat Constant")
	{
		setNumEntries(0);
		myMallocChar(mem, symTable, freeHeap, prime, "s", 10, "hett");
		
		myStrCat(mem, symTable, prime, "s", "\"llo\"");
	
		int indexOfBaseInMem = symTable[hashTableSearch(symTable, prime, "s")].offset;
		REQUIRE(strcmp(((char *) &mem[indexOfBaseInMem]), "hettllo") == 0);
	}

	SECTION("String Concat Constant Longer")
	{
		setNumEntries(0);
		myMallocChar(mem, symTable, freeHeap, prime, "s", 20, "hello");
		
		myStrCat(mem, symTable, prime, "s", "\" there!\"");
	
		int indexOfBaseInMem = symTable[hashTableSearch(symTable, prime, "s")].offset;
		REQUIRE(strcmp(((char *) &mem[indexOfBaseInMem]), "hello there!") == 0);
	}

	SECTION("String Concat Var")
	{
		setNumEntries(0);
		myMallocChar(mem, symTable, freeHeap, prime, "s", 6, "he");
		myMallocChar(mem, symTable, freeHeap, prime, "p", 6, "llo");
		
		myStrCat(mem, symTable, prime, "s", "p");

		int indexOfBaseInMem = symTable[hashTableSearch(symTable, prime, "s")].offset;
		REQUIRE(strcmp(((char *) &mem[indexOfBaseInMem]), "hello") == 0);
	}

	SECTION("String Concat Var Longer")
	{
		setNumEntries(0);
		myMallocChar(mem, symTable, freeHeap, prime, "s", 18, "hello");
		myMallocChar(mem, symTable, freeHeap, prime, "p", 12, " there!");
		
		myStrCat(mem, symTable, prime, "s", "p");

		int indexOfBaseInMem = symTable[hashTableSearch(symTable, prime, "s")].offset;
		REQUIRE(strcmp(((char *) &mem[indexOfBaseInMem]), "hello there!") == 0);
	}

	free(mem);
	free(freeHeap);
	free(symTable);
}
