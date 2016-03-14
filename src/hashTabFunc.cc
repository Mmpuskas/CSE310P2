#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "defs.h"

int numEntries = 0;
void setNumEntries(int num)
{
	numEntries = num;
}

// Type of each entry is assumed to be initialized to -1
// When an item is removed, the type of the empty spot is set to -2

// Input: The string that you want to hash
// Output: The hash of that string
int hashFunc(char const* symbol, int prime)
{
	int strVal = 0;
	int index = 0;
	while(symbol[index] != 0)
	{
		strVal += (int) symbol[index];
		index++;
	}

	return strVal % prime;
}

// Input: The data you want to store, and the symbol table to store it in
// Output: The symbol table contains the data at an appropriate position if it was not already full, no return
void hashTableInsert(struct symbolTableEntry* symTable, int prime, char const* symbol, int type, int offs, int bytes)
{
	int hash = hashFunc(symbol, prime);

	if(numEntries < prime)
	{
		numEntries++;
		int index = hash;

		while(symTable[index].type != -1 && symTable[index].type != -2)
		{
			index++;		
			if(index == prime)
				index = 0;	
		}

		strcpy(symTable[index].symbol, symbol);
		symTable[index].type = type;
		symTable[index].offset = offs;
		symTable[index].noBytes = bytes;
	}		
	else
		printf("ERROR: Symbol table is full.");

}

// Input: Symbol table to search in, prime number to hash by, symbol to search for
// Output: If symbol was found, the type of it will be set to -2. No return.
void hashTableRemove(struct symbolTableEntry* symTable, int prime, char const* symbol)
{
	if(numEntries > 0)
	{
		int index = hashTableSearch(symTable, prime, symbol);

		numEntries--;

		if(index != -1)
			symTable[index].type = -2;
		else
			printf("ERROR: Symbol not found");
	
	}
	else
		printf("ERROR: Table is empty");
}

// Input: Symbol table to search in, prime number to hash by, symbol to search for
// Output: The index in the symbol table where the symbol was found, -1 if not found
int hashTableSearch(struct symbolTableEntry* symTable, int prime, char const* symbol)
{
	int hash = hashFunc(symbol, prime);
	int indexOfSymbol = -1;

	if(numEntries > 0)
	{
		int index = hash;		
		int found = 0;

		while(symTable[index].type != -1 && found == 0)
		{
			if(symTable[index].type != -2)
			{
				if(strcmp(symTable[index].symbol, symbol) == 0)		
				{
					found = 1;	
					indexOfSymbol = index;	
				}
			}
			index++;		
			if(index == prime)
				index = 0;	
		}
	}	
	else
		printf("ERROR: Symbol table is empty.");

	return indexOfSymbol;
}
