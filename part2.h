/*****************************************************************************

 File        : part2.h

 Date        : 5th December 2019

 Description :  Header file for implementation of a memory manager with best fit,
                worst fit and next fit algorithms in part2.c


 Authors      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#ifndef AOCCW2_PART2_H
#define AOCCW2_PART2_H

#include <stddef.h>
#include <stdbool.h>

//Function pointer to requested allocate function
void* (*allocate)(size_t bytes);

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
 * @param algorithm string to set function pointer for allocate algorithm
 */
void initialise ( void *memory , size_t size, char *algorithm );

/**
 * Returns a segment of dynamically allocated memory of the specified size in bytes.
 * Uses best fit algorithm - finds smallest suitable hole available in list.
 * Hole split into new segment to be returned and a smaller hole + new node created.
 * If there is only space for the memory to be allocated, no new node is created.
 * @param bytes size of memory requested
 * @return pointer to start of memory allocated, or null if no memory available
 */
void *allocateBestFit (size_t bytes );

/**
 * Returns a segment of dynamically allocated memory of the specified size in bytes.
 * Uses worst fit algorithm - finds largest hole available in list.
 * Hole split into new segment to be returned and a smaller hole + new node created.
 * If there is only space for the memory to be allocated, no new node is created.
 * @param bytes size of memory requested
 * @return pointer to start of memory allocated, or null if no memory available
 */
void *allocateWorstFit (size_t bytes );

/**
 * Returns a segment of dynamically allocated memory of the specified size in bytes.
 * Uses next fit algorithm - starts at last visited node scans along list until the
 * first sufficiently large hole identified. Hole split into new segment to be returned
 * and a smaller hole + new node created. If there is only space for the memory to be allocated,
 * no new node is created.
 * @param bytes size of memory requested
 * @return pointer to start of memory allocated, or null if no memory available
 */
void *allocateNextFit (size_t bytes );

/**
 * Function to free a block of dynamically allocated memory given a pointer to the start of the memory.
 * @param memory pointer to start of memory to be deallocated
 */
void deallocate ( void *memory );

/**
 * Allocate node function used by all 3 different allocate functions.
 * @param node Node for memory to be allocated too
 * @param bytes size of memory requested
 * @return address of the start of memory allocated
 */
Node *allocateNode(Node *node, int bytes);

/**
 * Helper function to print all nodes created in linked list
 */
void printNode();

#endif //AOCCW2_PART2_H
