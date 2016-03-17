#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "defs.h"

int k = 5;
int maxHeapSize = pow(2, k)/4;
int heapSize = 0;

// Input: Size to set heap to
// Output: heapSize = size
void setHeapSize(int size)
{
	heapSize = size;
}
// Input: void
// Output: The size of the heap
int getHeapSize()
{
	return heapSize;
}
// Input: The desired size of the heap
// Output: A pointer to a heapEntry array with appropriate size while excluding A[0]
struct heapEntry* initHeap(int size)
{
	heapSize = 0;
	struct heapEntry* freeHeap = (struct heapEntry*) malloc((size + 1) * sizeof(struct heapEntry));	
	return freeHeap;
}
// Input: The index of the node to check
// Output the index of the parent of the node if it exists, else -1
int returnParent(int index)
{
	return (index == 1) ? -1 : (index / 2);
}
// Input: The index of the node to check
// Output: The index of the left child if it exists, else -1
int returnLeft(int index)
{
	return (index*2 <= heapSize) ? (index*2) : -1;
}
// Input: The index of the node to check
// Output: The index of the right child if it exists, else -1
int returnRight(int index)
{
	return (index*2+1 <= heapSize) ? (index*2+1) : -1;
}
// Input: The two nodes to be swapped 
// Output: The nodes are swapped, no return 
void swap(struct heapEntry* l, struct heapEntry* r)
{
	struct heapEntry temp = *l;
	*l = *r;
	*r = temp;	
}
// Input: A pointer to a heap, the index to start at
// Output: Every node below the index maintains the heap propery, no return
void maxHeapify(struct heapEntry* heapArr, int index)
{
	int left = returnLeft(index);	
	int right = returnRight(index);	
	int largest = 0;

	if(left > 0 && left <= heapSize && heapArr[left].blockSize > heapArr[index].blockSize)
		largest = left;
	else
		largest = index;

	if(right > 0 && right <= heapSize && heapArr[right].blockSize > heapArr[largest].blockSize)
		largest = right;
	
	if(largest != index)
	{
		swap(&heapArr[index], &heapArr[largest]);
		maxHeapify(heapArr, largest);
	}
}
// Input: A pointer to a heap to-be
// Output: The heap to-be is now a pretty heap
void buildMaxHeap(struct heapEntry* heapArr)
{
	int mid = heapSize/2;
	for(int i = mid; i >= 1; i--)
		maxHeapify(heapArr, i);	
}
// Input: A pointer to a heap, the data to insert
// Output: A node containing the data is inserted into the heap, heap property is maintained, no return
void maxHeapInsert(struct heapEntry* heapArr, int blockSize, int offset)
{
	if(heapSize < maxHeapSize)
	{
		heapSize++;
		heapArr[heapSize].blockSize = blockSize;
		heapArr[heapSize].offset = offset;

		int insertIndex = heapSize;
		while(heapArr[insertIndex].blockSize > heapArr[returnParent(insertIndex)].blockSize)
		{
			swap(&heapArr[insertIndex], &heapArr[returnParent(insertIndex)]);
			insertIndex = returnParent(insertIndex);
		}
	}
	else
		printf("Heap is full\n");
}
// Input: A heap to extract from
// Output: The max value of the heap: The heap maintains the max heap property
struct heapEntry heapExtractMax(struct heapEntry* heapArr)
{
	struct heapEntry max = heapArr[1];

	heapArr[1] = heapArr[heapSize];
	heapSize--;
	maxHeapify(heapArr, 1);	
	return max;
}
// Prints the current state of the heap
void printHeap(struct heapEntry* heapArr)
{
	for(int i = 1; i <= heapSize; i++)
		printf("(%d, %d)", heapArr[i].blockSize, heapArr[i].offset);	

	printf("\n");
}
