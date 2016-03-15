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
#include "defs.h"

int prime = 0;
int numCom = 0;

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
//	char* linePointer = (char*) malloc(150); //A typical long line in the sample is 96 chars, 150 should be safe. 
	
}

