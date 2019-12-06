/*****************************************************************************

 File        : part2.h

 Date        : 5th December 2019

 Description : Header file for implementation of a memory manager with best fit,
                worst fit and next fit algorithms

 Author      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#ifndef AOCCW2_PART2_H
#define AOCCW2_PART2_H

#include <stddef.h>

typedef struct Node Node;

void initialise ( void *memory , size_t size, char *algorithm );

void *allocateBestFit (size_t bytes );

void *allocateWorstFit (size_t bytes );

void *allocateNextFit (size_t bytes );

void deallocate ( void *memory );

void printNode();

#endif //AOCCW2_PART2_H
