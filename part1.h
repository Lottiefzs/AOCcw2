/*****************************************************************************
 File        : part1.h

 Date        : 5th December 2019

 Description :  Header file for implementation of a basic memory manager, using the
                "First Fit" strategy.

 Author      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#ifndef AOCCW2_PART1_H
#define AOCCW2_PART1_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node{
    //Points to next node in list
    struct Node * next;
    //Points to previous node in list
    struct Node * previous;
    //States if memory after node is free
    bool isFree;
    //Size of memory allocated to node
    size_t size;
}Node;

/**
 * Given a pointer to a block of memory initialise the first node in the memory manager
 * to the size of the heap (in bytes) allowing for the size of the first node.
 * @param memory pointer to heap address
 * @param size size of heap
 */
void initialise (void *memory , size_t size);

/**
 * Returns a segment of dynamically allocated memory of the specified size in bytes.
 * Uses first fit algorithm - scans along list until the first sufficiently large hole
 * identified. Hole split into new segment to be returned and a smaller hole + new node.
 * If there is only space for the memory to be allocated, no new node is created.
 * @param bytes size of memory requested
 * @return pointer to start of memory allocated, or null if no memory available
 */
void *allocate (size_t bytes);

/**
 * Function to free a block of dynamically allocated memory given a pointer to the start of the memory.
 * @param memory pointer to start of memory to be deallocated
 */
void deallocate (void *memory);

/**
 * Helper function to print all nodes created in linked list
 */
void printNode();

#endif //AOCCW2_PART1_H
