//
// Created by Student on 02/12/2019.
//

#ifndef AOCCW2_PART2_H
#define AOCCW2_PART2_H

#include <stddef.h>

typedef struct Node Node;

void initialise ( void *memory , size_t size, char *algorithm );

void *allocateBestFit (size_t bytes );

void *allocateWorstFit (size_t bytes );

void *allocateNextFit (size_t bytes );

void deallocate ( void *memory );

#endif //AOCCW2_PART2_H
