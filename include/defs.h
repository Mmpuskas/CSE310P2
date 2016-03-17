#pragma once

#define SYMBOL_LENGTH   20      // Maximum length of a variable name (identifier)
#define INT             0       // Integer type
#define CHAR            1       // Character string type
#define BST             2       // Binary search tree type
#define STRTERM         '\0'    // String termination character

struct heapEntry{
    int blockSize;  // Size of free block in bytes
    int offset;     // Start address of free block given as offset into memory block
};

struct symbolTableEntry{
    char symbol[ SYMBOL_LENGTH ]; // Variable identifier
    int type;       // The type of the variable: one of INT, CHAR, or BST
    int offset;     // Start address of variable in physical memory given as offset
    int noBytes;    // Size in bytes used to store variable in physical memory
};

struct bstNode{
    int key; // A node of a binary search tree ordered on this key value
    int left;  // Left child
    int right; // Right child
};

//Heap Functions
struct heapEntry* initHeap(int size);
void maxHeapInsert(struct heapEntry* heapArr, int blockSize, int offset);
struct heapEntry heapExtractMax(struct heapEntry* heapArr);
void maxHeapify(struct heapEntry* heapArr, int index);
void setHeapSize(int size);
void buildMaxHeap(struct heapEntry* heapArr);

//Hash Table Functions
void setNumEntries(int num);
int hashFunc(char const* symbol, int prime);
void hashTableInsert(struct symbolTableEntry* symTable, int prime, char const* symbol, int type, int offs, int bytes);
void hashTableRemove(struct symbolTableEntry* symTable, int prime, char const* symbol);
int hashTableSearch(struct symbolTableEntry* symTable, int prime, char const* symbol);

//Control Path Functions
char* initMem(int memSize);
void myMallocInt(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, char const* varName, unsigned int value);
void myMallocChar(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, char const* varName, int len, char const* value);
void map(char* mem, int len);
void myFree(char* mem, struct symbolTableEntry* symTable, struct heapEntry* freeHeap, int prime, char const* varName);
void myAdd(char* mem, struct symbolTableEntry* symTable, int prime, char const* varName, char const* vName);
void myCompact(struct heapEntry* freeHeap);
int getHeapSize();
void myStrCat(char* mem, struct symbolTableEntry* symTable, int prime, char const* sBase, char const* sToAdd);
void printVar(char* mem, struct symbolTableEntry* symTable, int prime, const char* varName);

//Support Functions
void readLine(char *linePointer);
void init(void);

