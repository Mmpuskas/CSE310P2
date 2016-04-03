#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

// Input: Pointer to memspace, length of memspace
// Output: Prints a map of the memspace, no return
void map(char* mem, int len)
{
	int lineWidth = 16;

	printf("Hex Map\n");
	printf("       | 0  1  2  3  | 4  5  6  7  | 8  9  A  B  | C  D  E  F  |");
	for(int i = 0; i < len; i++)
	{
		if(i % lineWidth == 0 && i > 0)
			printf("|\n0x%04X | ", i);	
		else if(i % lineWidth == 0)
			printf("\n0x%04X | ", i);	
		else if(i % 4 == 0)
			printf("| ");

		printf("%02X ",(unsigned char) mem[i]);	
	}
	printf("|");

	printf("\n\nChar Map\n");
	printf("       | 0  1  2  3  | 4  5  6  7  | 8  9  A  B  | C  D  E  F  |");
	for(int i = 0; i < len; i++)
	{
		if(i % lineWidth == 0 && i > 0)
			printf("|\n0x%04X | ", i);	
		else if(i % lineWidth == 0)
			printf("\n0x%04X | ", i);	
		else if(i % 4 == 0)
			printf("| ");

		if((mem[i] < ':' && mem[i] > '/') || mem[i] == 0)
			printf("%02x ", mem[i]);	
		else
			printf("%2c ", mem[i]);	
	}
	printf("|");
	printf("\n");
}
// Input: Pointer to memspace, pointer to symbol table, pointer to freespace heap, prime number being used, var name, length of string, value
// Output: Memspace contains value, symbol table contains symbol and location, space of variable is removed from heap
void myMallocInt(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, char const* varName, unsigned int value)
{
	struct heapEntry topOfHeap = heapExtractMax(freeHeap);
	
	if(topOfHeap.blockSize >= 4)
	{
		//Update the symbol table
		hashTableInsert(symTable, prime, varName, INT, topOfHeap.offset, 4);

		//Place the variable in the memspace
		*((unsigned int*) &mem[topOfHeap.offset]) = value; 

		//Update the free heap
		maxHeapInsert(freeHeap, topOfHeap.blockSize - 4, topOfHeap.offset + 4);
	}	
	else
	{
		maxHeapInsert(freeHeap, topOfHeap.blockSize, topOfHeap.offset);	
		printf("ERROR: Not enough space to allocate variable.\n");
	}
}
// Input: Pointer to memspace, pointer to symbol table, pointer to freespace heap, prime number being used, var name, length of string, value
// Output: Memspace contains value, symbol table contains symbol and location, space of variable is removed from heap
void myMallocChar(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, char const* varName, int len, char const* value)
{
	struct heapEntry topOfHeap = heapExtractMax(freeHeap);
	int adjustedLen = len;
	if(len % 4 != 0)
		adjustedLen += (4 - (len % 4));
	
	if(topOfHeap.blockSize >= adjustedLen)
	{
		//Update the symbol table
		hashTableInsert(symTable, prime, varName, CHAR, topOfHeap.offset, len);

		//Place the variable in the memspace
		if(value)
		{
			strcpy(((char*) &mem[topOfHeap.offset]), value);
			mem[topOfHeap.offset + len] = STRTERM; //Null terminator
		}

		//Update the free heap
		maxHeapInsert(freeHeap, topOfHeap.blockSize - adjustedLen, topOfHeap.offset + adjustedLen);
	}	
	else
	{
		maxHeapInsert(freeHeap, topOfHeap.blockSize, topOfHeap.offset);	
		printf("ERROR: Not enough space to allocate variable.\n");
	}
}
// Input: Pointer to memspace, pointer to symbol table, pointer to freespace heap, prime number being used, var name, length of string, value
// Output: Memspace contains value, symbol table contains symbol and location, space of variable is removed from heap
void myMallocBST(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, char const* varName, unsigned int value)
{
	struct heapEntry topOfHeap = heapExtractMax(freeHeap);
	
	if(topOfHeap.blockSize >= 12)
	{
		//Update the symbol table
		hashTableInsert(symTable, prime, varName, BST, topOfHeap.offset, 12);

		//Place the variable in the memspace
		((struct bstNode*) &mem[topOfHeap.offset])->key = value; 
		((struct bstNode*) &mem[topOfHeap.offset])->left = -1; 
		((struct bstNode*) &mem[topOfHeap.offset])->right = -1; 

		//Update the free heap
		maxHeapInsert(freeHeap, topOfHeap.blockSize - 12, topOfHeap.offset + 12);
	}	
	else
	{
		maxHeapInsert(freeHeap, topOfHeap.blockSize, topOfHeap.offset);	
		printf("ERROR: Not enough space to allocate variable.\n");
	}
}
// Input: A bstNode's root in memory
// Output: Navigates the BST to free every node. Root still exists in symbol table.
void freeBST(char* mem, struct heapEntry* freeHeap, int indexInMem)
{
	int leftChild = 0;
	int rightChild = 0;
	leftChild = ((struct bstNode*) &mem[indexInMem])->left;
	rightChild = ((struct bstNode*) &mem[indexInMem])->right;

	//Recurse on children
	if(leftChild != -1)
		freeBST(mem, freeHeap, leftChild);
	if(rightChild != -1)
		freeBST(mem, freeHeap, rightChild);

	//Return space to free heap
	maxHeapInsert(freeHeap, 12, indexInMem);

	//Re-write the free space in mem
	for(int i = indexInMem; i < indexInMem+12; i++)
		mem[i] = '~';

}
// Input: Pointer to memspace, pointer to symbol table, pointer to freespace heap, prime number being used, var name
// Output: Var is removed from symbol table if it was found, else no change. 
void myFree(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, char const* varName)
{
	int indexOfSymbol = 0;
	indexOfSymbol = hashTableSearch(symTable, prime, varName);

	if(indexOfSymbol != -1)
	{
		if(symTable[indexOfSymbol].type == 2) //Type is BST
		{
			//Call recursive BST free
			freeBST(mem, freeHeap, symTable[indexOfSymbol].offset);
			
			//Remove from the symbol table
			hashTableRemove(symTable, prime, varName);
		}
		else //Type is INT or CHAR
		{
			int indexInMem = symTable[indexOfSymbol].offset;
			int sizeInMem = symTable[indexOfSymbol].noBytes;
			if(sizeInMem % 4 != 0)
				sizeInMem += (4 - (sizeInMem % 4));

			//Return space to free heap
			maxHeapInsert(freeHeap, sizeInMem, indexInMem);	

			//Re-write the free space in mem
			for(int i = indexInMem; i < indexInMem + sizeInMem; i++)
				mem[i] = '~';

			//Remove from the symbol table
			hashTableRemove(symTable, prime, varName);
		}
	}
	else
		printf("ERROR: Cannot free variable that has not been allocated\n");	
}
// Input: Pointer to memspace, pointer to symbol table, var name, var name / scalar
// Output: Value of scalar or second var is added to first
void myAdd(char* mem, struct symbolTableEntry* symTable, int prime, char const* varName, char const* vName)
{
	int varIndex1 = 0;
	varIndex1 = hashTableSearch(symTable, prime, varName);

	if(varIndex1 != -1 && symTable[varIndex1].type == INT) //Var1 exists in symbol table
	{
		if(vName[0] > 47 && vName[0] < 58) //vName is a scalar
		{
			int value = atoi(vName);
			*((unsigned int*) &mem[symTable[varIndex1].offset]) += value; 
		}
		else //vName is a variable name
		{
			int varIndex2 = 0;
			varIndex2 = hashTableSearch(symTable, prime, vName);

			if(varIndex2 != -1 && symTable[varIndex2].type == INT) //Var2 exists in symbol table
			{
				*((unsigned int*) &mem[symTable[varIndex1].offset]) += *((unsigned int*) &mem[symTable[varIndex2].offset]);
			}
			else
				printf("ERROR: RHS does not exist in symbol table, or is not of type INT\n");	
		}
	}
	else
		printf("ERROR: LHS does not exist in symbol table, or is not of type INT\n");
}
// Input: Pointer to free heap
// Output: Adjacent free blocks have been coalesced
void myCompact(struct heapEntry* freeHeap)
{
	int numCompacted = 0;
	int heapSize = getHeapSize();
	struct heapEntry* heapArr = (struct heapEntry*) malloc(heapSize * sizeof(struct heapEntry));	
	
	//Move to new array
	for(int i = 0; i < heapSize; i++)
		heapArr[i] = heapExtractMax(freeHeap);

	//Sort
	struct heapEntry temp;
	for(int i = 0; i < heapSize-1; i++)
		for(int j = i+1; j < heapSize; j++)
			if(heapArr[i].offset > heapArr[j].offset)
			{
				temp = heapArr[j];
				heapArr[j] = heapArr[i];
				heapArr[i] = temp;		
			}	
	
	//Coalesce
	for(int i = 0; i < heapSize; i++)
	{
		if(heapArr[i].offset + heapArr[i].blockSize == heapArr[i+1].offset)
		{
			heapArr[i].blockSize += heapArr[i+1].blockSize;
			heapArr[i+1].offset = -1;		
			numCompacted++;
		}
	}
	
	//Push back into heap
	for(int i = 0; i < heapSize; i++)
		if(heapArr[i].offset != -1)
		{
			maxHeapInsert(freeHeap, heapArr[i].blockSize, heapArr[i].offset);	
		}
	if(numCompacted > 0)
		myCompact(freeHeap);	
	else
	{
		printf("Listing free blocks sorted by offset:\n");	
		for(int i = 0; i < heapSize; i++)
			printf("Offset: %d BlockSize: %d\n", heapArr[i].offset, heapArr[i].blockSize);
		printf("\n");
	}

	free(heapArr);
}
// Input: Relevant data structures, prime, name of base var, literal of var to add
// Output: literal is concatenated to sBase in mem
void myStrCatConst(char* mem, struct symbolTableEntry* symTable, int prime, char const* sBase, char const* sToAdd)
{
	int varIndex = hashTableSearch(symTable, prime, sBase);

	if(varIndex != -1 && symTable[varIndex].type == CHAR)
	{
		int lenBase = strlen(((char*) &mem[symTable[varIndex].offset]));	
		int lenToAdd = strlen(sToAdd);	

		int bytesAllocated = symTable[varIndex].noBytes;
		if(bytesAllocated % 4 != 0) //Word align
			bytesAllocated += (4 - (bytesAllocated % 4));

		if((bytesAllocated - lenToAdd - lenBase) >= 0)
		{
			int indexOfBaseInMem = symTable[varIndex].offset;

			//Concatenate
			strcpy(((char*) &mem[indexOfBaseInMem + lenBase]), sToAdd);
			mem[indexOfBaseInMem + lenBase + lenToAdd] = STRTERM; //Null terminator
		}
		else
			printf("ERROR: LHS insufficient length to perform strcat\n");
	}
	else
		printf("ERROR: LHS not found in symbol table or not type CHAR\n");
}
// Input: Relevant data structures, prime, name of base var, name of var to add
// Output: var is concatenated to sBase in mem
void myStrCatVar(char* mem, struct symbolTableEntry* symTable, int prime, char const* sBase, char const* sToAdd)
{
	int baseIndex = hashTableSearch(symTable, prime, sBase);
	int toAddIndex = hashTableSearch(symTable, prime, sToAdd);

	if(baseIndex != -1 && symTable[baseIndex].type == CHAR)
	{
		if(toAddIndex != -1 && symTable[toAddIndex].type == CHAR)
		{
			int lenToAdd = strlen(((char*) &mem[symTable[toAddIndex].offset]));
			int lenBase = strlen(((char*) &mem[symTable[baseIndex].offset]));

			int bytesAllocated = symTable[baseIndex].noBytes;
			if(bytesAllocated % 4 != 0) //Word align
				bytesAllocated += (4 - (bytesAllocated % 4));

			if((bytesAllocated - lenToAdd - lenBase) >= 0)
			{
				int indexOfBaseInMem = symTable[baseIndex].offset;
				int indexOfToAddInMem = symTable[toAddIndex].offset;

				//Concatenate
				strcpy(((char*) &mem[indexOfBaseInMem + lenBase]), ((char*) &mem[indexOfToAddInMem]));
				mem[indexOfBaseInMem + lenBase + lenToAdd] = STRTERM; //Null terminator
			}
			else
				printf("ERROR: LHS insufficient length to perform strcat\n");
		}
		else
			printf("ERROR: RHS not found in symbol table or not type CHAR\n");
	}
	else
		printf("ERROR: LHS not found in symbol table or not type CHAR\n");
}
// Input: Relevant data structures, prime, name of base var, literal or name of var to add
// Output: sToAdd is concatenated to sBase in mem
void myStrCat(char* mem, struct symbolTableEntry* symTable, int prime, char const* sBase, char const* sToAdd)
{
	if(sToAdd[0] == '\"')
	{
		int len = strlen(sToAdd);

		char* newToAdd = (char*) malloc(len * sizeof(char));
		strcpy(newToAdd, sToAdd);
		for(int i = 0; i < len - 1; i++)
			newToAdd[i] = newToAdd[i+1];
		newToAdd[len - 2] = STRTERM;

		myStrCatConst(mem, symTable, prime, sBase, newToAdd);

		free(newToAdd);
	}
	else
		myStrCatVar(mem, symTable, prime, sBase, sToAdd);
}
// Input: A bstNode's root in memory
// Output: Prints the in-order traversal of the tree 
void printTreeNode(char* mem, struct bstNode* root)
{
	if(root->left != -1)
		printTreeNode(mem, ((struct bstNode*) &mem[root->left]));
	printf("%d\t", root->key);
	if(root->right != -1)
		printTreeNode(mem, ((struct bstNode*) &mem[root->right]));
}
// Input: Relevant data structures, prime, name of var to print
// Output: prints value of var
void printVar(char* mem, struct symbolTableEntry* symTable, int prime, const char* varName)
{
	int indexOfSymbol = 0;
	indexOfSymbol = hashTableSearch(symTable, prime, varName);

	if(indexOfSymbol != -1 && symTable[indexOfSymbol].type != -2)
	{
		if(symTable[indexOfSymbol].type == CHAR)
			printf("%s = %s\n", varName, ((char*) &mem[symTable[indexOfSymbol].offset]));
		else if (symTable[indexOfSymbol].type == INT)
			printf("%s = %d\n", varName,  mem[symTable[indexOfSymbol].offset]);
		else if (symTable[indexOfSymbol].type == BST)
		{
			printTreeNode(mem, ((struct bstNode*) &mem[symTable[indexOfSymbol].offset]));
			printf("\n");
		}
		else
			printf("ERROR: Type is not CHAR or INT\n");
	}
	else
		printf("ERROR: Symbol not found.\n");
}
// Input: Relevant data structures, prime, value of node to insert
// Output: If there is room in the memspace, the node is added to the BST. 
void bstInsert(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, const char* rootName,  int value)
{
	int indexOfRoot = 0;
	indexOfRoot = hashTableSearch(symTable, prime, rootName);

	if(indexOfRoot != -1 && symTable[indexOfRoot].type == 2)
	{
		struct heapEntry topOfHeap = heapExtractMax(freeHeap);
		
		if(topOfHeap.blockSize >= 12)
		{
			//Place the variable in the memspace
			((struct bstNode*) &mem[topOfHeap.offset])->key = value; 
			((struct bstNode*) &mem[topOfHeap.offset])->left = -1; 
			((struct bstNode*) &mem[topOfHeap.offset])->right = -1; 

			//Set it as a child of the BST
			int insertIndex = symTable[indexOfRoot].offset;
			int foundFlag = 0;
			int leftChild = 0;
			int rightChild = 0;

			while(foundFlag == 0)
			{
				leftChild = ((struct bstNode*) &mem[insertIndex])->left;
				rightChild = ((struct bstNode*) &mem[insertIndex])->right;
				if(value < ((struct bstNode*) &mem[insertIndex])->key) //Node belongs in left tree
					if(leftChild == -1)
					{
						((struct bstNode*) &mem[insertIndex])->left = topOfHeap.offset;
						foundFlag = 1;	
					}
					else
						insertIndex = leftChild;
				else if(value > ((struct bstNode*) &mem[insertIndex])->key) //Node belongs in right tree
				{
					if(rightChild == -1)
					{
						((struct bstNode*) &mem[insertIndex])->right = topOfHeap.offset;
						foundFlag = 1;	
					}
					else
						insertIndex = rightChild;
				}
			}

			//Update the free heap
			maxHeapInsert(freeHeap, topOfHeap.blockSize - 12, topOfHeap.offset + 12);
		}	
		else
		{
			maxHeapInsert(freeHeap, topOfHeap.blockSize, topOfHeap.offset);	
			printf("ERROR: Not enough space to allocate variable.\n");
		}
	}
	else
		printf("ERROR: Can only insert into BST.\n");
}
