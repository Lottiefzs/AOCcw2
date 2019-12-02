//
// Created by charl on 28/11/2019.
//

#ifndef AOCCW2_PART1_H
#define AOCCW2_PART1_H

typedef struct Node Node;

void initialise ( void *memory , size_t size );

void *allocate ( size_t bytes );

void deallocate ( void *memory );

#endif //AOCCW2_PART1_H
