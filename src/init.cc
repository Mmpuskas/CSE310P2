/*
 * ##  Michael Puskas  ##
 * ##  CSE 310 S 2016  ##
 * ## mmpuskas@asu.edu ##
 *
 * CSE 310 Project 2
 * Memory Management Project
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defs.h"

// Input: Size of memory space
// Output: Pointer to beginning of memory space
char* initMem(int memSize)
{
	char* memPointer = (char*) calloc(memSize, sizeof(char*));	

	return memPointer;
}

/* init()
 *
 * Main function to control flow of program.
 * Program will:
 * 	- Read commands from stdin
 * 	- Initialize array of chars to use as a memory space
 * 	- Manage memory space according to commands
 */
void init(void)
{
	printf(KBLU "\n## Welcome to myMemoryHandler. ##\n\n" RESET);
	printf(KRED "Initializing Variables\n");
	char* linePointer = (char*) malloc(150); //A typical long line in the sample is 96 chars, 150 should be safe. 
	
	//Read first two lines
	readLine(linePointer);
	int memSize = pow(2, atoi(linePointer));
	readLine(linePointer);
	int prime = atoi(linePointer);

	//Instantiate and initialize variables
	char* mem = initMem(memSize);	
	int heapSize = memSize / 4;
    	struct heapEntry* freeHeap = initHeap(heapSize); 
	maxHeapInsert(freeHeap, memSize, 0);

	struct symbolTableEntry* symTable = (struct symbolTableEntry*) malloc(prime * sizeof(symbolTableEntry));
	for(int i = 0; i < prime; i++)
		symTable[i].type = -1;
	
	//Read third line, prepare for command processing
	readLine(linePointer);	
	int numCommands = atoi(linePointer);
	char delim[2] = " ";

	printf("Variables Initialized. Processing %d Commands:\n\n" RESET, numCommands);
	for (int i = 0; i < numCommands; i++)
	{
		readLine(linePointer);
		char* command = strtok(linePointer, delim);

		if(strcmp(command, "allocate") == 0)
		{
			command = strtok(NULL, delim);
			if(strcmp(command, "INT") == 0)
			{
				char* varName = strtok(NULL, delim);
				printf("Allocating Integer \"%s\"\n", varName);

				int val = atoi(strtok(NULL, delim));
				
				myMallocInt(mem, symTable, freeHeap, prime, varName, val);
			}
			else if(strcmp(command, "CHAR") == 0)
			{
				char* varName = strtok(NULL, delim);
				printf("Allocating String \"%s\"\n", varName);

				int len = atoi(strtok(NULL, delim));
				
				char* sValue = strtok(NULL, "\"");
				
				myMallocChar(mem, symTable, freeHeap, prime, varName, len, sValue);
			}
			else if(strcmp(command, "BST") == 0)
			{
				char* varName = strtok(NULL, delim);
				printf("Allocating BST \"%s\"\n", varName);

				int val = atoi(strtok(NULL, delim));
				
				myMallocBST(mem, symTable, freeHeap, prime, varName, val);
			}
			else
			{
				printf("ERROR: Improper type on allocate command");
				return;
			}
			printf("Allocate Complete.\n\n");
		}
		else if(strcmp(command, "free") == 0)
		{
			char* varName = strtok(NULL, delim);
			printf("Deallocating variable \"%s\"\n", varName);
		
			myFree(mem, symTable, freeHeap, prime, varName);
			
			printf("Deallocate Complete.\n\n");
		}
		else if(strcmp(command, "add") == 0)
		{
			char* varName = strtok(NULL, delim);
			char* varToAdd = strtok(NULL, delim);
			printf("Adding %s to %s\n", varToAdd, varName);

			myAdd(mem, symTable, prime, varName, varToAdd);

			printf("Add Complete\n\n");	
		}
		else if(strcmp(command, "map") == 0)
		{
			printf("Mapping the memory space:\n\n");
			map(mem, memSize);
			printf("\n\n");
		}
		else if(strcmp(command, "print") == 0)
		{
			char* varName = strtok(NULL, delim);
			printf("Printing variable \"%s\"\n", varName);

			printVar(mem, symTable, prime, varName);
		
			printf("\n\n");
		}
		else if(strcmp(command, "compact") == 0)
		{
			printf("Compacting the freespace heap:\n");

			myCompact(freeHeap);
		
			printf("Compaction Complete\n\n");
		}
		else if(strcmp(command, "strcat") == 0)
		{
			char* varName = strtok(NULL, delim);
			char* varToAdd = strtok(NULL, "\n");
			printf("Concatenating %s to %s\n", varToAdd, varName);

			myStrCat(mem, symTable, prime, varName, varToAdd);

			printf("Concatenation complete\n\n");	
		}
		else if(strcmp(command, "insert") == 0)
		{
			char* rootName = strtok(NULL, delim);
			int value = atoi(strtok(NULL, "\n"));
		
			printf("Inserting \"%d\" into BST \"%s\".\n", value, rootName);
			bstInsert(mem, symTable, freeHeap, prime, rootName, value);
			printf("Insert complete\n\n");
		}
	}
}

