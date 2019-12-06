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

typedef struct Node Node;

/**
 * Initialise the memory manager from a pointer to a block of memory the size of the heap size given in bytes.
 * @param memory
 * @param size
 */
void initialise ( void *memory , size_t size );

/**
 * Returns a segment of dynamically allocated memory of the specified size in bytes.
 * @param bytes size of memory requested
 * @return pointer to start of memory allocated, or null if no memory available
 */
void *allocate ( size_t bytes );

/**
 * Used to free a block of dynamically allocated memory
 * @param memory pointer to start of memory to be deallocated
 */
void deallocate ( void *memory );

void printNode();

#endif //AOCCW2_PART1_H
