/*
 * memorySim.c
 *
 *  Updated on: 12 Nov 2020
 *      Author: rparker
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freelist.h"

FILE * fd; // input file descriptor 
bool done = false;  

const int MAX_SIZE = 10000;
MemBlock allocated[MAX_SIZE];
int alloc_index = 0; 
int addr, size; 
char kind[10]; 


void doSimulation(bool first)
{ 
	MemBlock firstBlock;
	firstBlock.startingAddr = 0; 
	firstBlock.size = BLOCK_SIZE;
	insertFirst(firstBlock); 
	
	// blockAddr is to be calculated for uniqueness
	int blockAddr = addr;
	int blockSize = size; 
	
	// if true, then first
	// if false, then best
	// else, error
	if (first) {
		printf("Running first...");
		if (strcmp(kind, "ALLOC") == 0)
		{
			// allocate memory
			printf("Action: ALLOC id: %d size: %d", addr, size);
			// find a block that is large enough
			// where firstBlock.size >= blockSize
			// split it into two -- the allocation and the remainder
			// put the allocation block into the allocated array
			
			Node * ptr; 
			for (ptr = getFirst(); ptr != NULL; ptr = ptr->next) {
				if (ptr->block.size >= blockSize) {
					// there is a large enough block here.
					// put the new block here
					MemBlock newBlock; 
					newBlock.size = blockSize;
					newBlock.startingAddr = ptr->block.startingAddr;
					
					allocated[addr] = newBlock;
					
					ptr->block.startingAddr = ptr->block.startingAddr + blockSize;
					ptr->block.size = ptr->block.size - blockSize; 
				} 
				/**if (ptr->block.size > blockSize) { 
					// add another block to the freelist
						MemBlock addBlock;
						addBlock.startingAddr = 0; 
						addBlock.size = BLOCK_SIZE;
						insertAfter(addBlock); 
				} */
			} 
			
		} else if (strcmp(kind, "FREE") == 0)
		{
			// free memory
			insertFirst(allocated[addr]); 
			allocated[addr].size = 0; 	
		} else 
		{ 
			printf("Error invalid operation: %s", kind);
		}
	} else if (!first) {
		printf("Running best...");
		if (strcmp(kind, "ALLOC") == 0)
		{
			
			Node * ptr; 
			Node * best; 
			int bestFit; 
			for (ptr = getFirst(); ptr != NULL; ptr = ptr->next) {
				// find a block that the memory might fit in
				if (ptr->block.size >= blockSize) {
					// is this place better than the last?
					if (ptr->block.size < bestFit) { 
						// if so, remember the size and save the ptr
						bestFit = ptr->block.size;
						best = ptr; 
					}
				} 
				/**if (ptr->block.size > blockSize) { 
					// add another block to the freelist
						MemBlock addBlock;
						addBlock.startingAddr = 0; 
						addBlock.size = BLOCK_SIZE;
						insertAfter(addBlock); 
				} */
			} 
			// allocate
			MemBlock newBlock; 
			newBlock.size = blockSize;
			newBlock.startingAddr = best->block.startingAddr;
					
			allocated[addr] = newBlock;
					
			best->block.startingAddr = best->block.startingAddr + blockSize;
			best->block.size = best->block.size - blockSize; 
			
		} else if (strcmp(kind, "FREE") == 0)
		{
			// free memory
			insertFirst(allocated[addr]); 
			allocated[addr].size = 0; 
		} else 
		{ 
			printf("Error invalid operation: %s", kind);
		}
	} else { 
		printf("Error...");
	} 

}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <FIRST|BEST> <allocationFile>\n", argv[0]);
		exit(1);
	}

	char * filename = argv[2];
    	fd = fopen(filename, "r");
	if (fd == NULL)
	{
		printf("Can't open input file: %s\n", filename);
		exit(1);
	}

	if (strcmp(argv[1], "FIRST") == 0)
	{
		printf("Running simulation for first\n");
		while (fscanf(fd, "%s %d %d", kind, &addr, &size) != EOF) 
			{ 
				doSimulation(true);
			}
		} 
	}
	else if (strcmp(argv[1], "BEST") == 0)
	{
		printf("Running simulation for best\n");
		while (fscanf(fd, "%s %d %d", kind, &addr, &size) != EOF) 
			{ 
				doSimulation(false); 
			}
		} 
	}
	else 
	{
		printf("Oops, bad memory type: %s", argv[1]);
	}

}