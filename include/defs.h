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

//Heap Function
struct heapEntry* initHeap(int size);
void max_heap_insert(struct heapEntry* heapArr, int blockSize, int offset);
struct heapEntry heap_extract_max(struct heapEntry* heapArr);
void max_heapify(struct heapEntry* heapArr, int index);
void setHeapSize(int size);
void build_max_heap(struct heapEntry* heapArr);

