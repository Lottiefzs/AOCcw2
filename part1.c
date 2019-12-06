/*****************************************************************************

 File        : part1.c

 Date        : 5th December 2019

 Description : Implementation of a basic memory manager, using the "First Fit"
                strategy. Providing three functions:
                - initalise() to initalise the memory manager from a pointer to a
                block of memory the size of the heap size given in bytes.
                - allocate() returns a segment of dynamically allocated memory of
                the specified size in bytes.
                - deallocate() - used to free a block of dynamically allocated memory

 Author      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "part1.h"

Node* first;

typedef struct Node{
    struct Node * next;
    struct Node * previous;
    bool isFree;
    size_t size;
}Node;

/**
 * Initialise the memory manager from a pointer to a block of memory the size of the heap size given in bytes.
 * @param memory
 * @param size
 */
void initialise ( void * memory , size_t size ){
    first = (Node*) memory;
    first->size = size - sizeof(Node);
    first->isFree = true;
    first->next = NULL;
    first->previous = NULL;
};

/**
 * Returns a segment of dynamically allocated memory of the specified size in bytes.
 * @param bytes size of memory requested
 * @return pointer to start of memory allocated, or null if no memory available
 */
void* allocate ( size_t bytes ){
    for (Node* node = first; node != NULL ; node = node->next) {
        if(node->isFree && node->size >= (bytes + sizeof(Node))){
            node->isFree = false;
            Node* new = (Node*) ((char*) node + bytes + sizeof(Node));
            new->next=node->next;
            node->next = new;
            new->previous = node;
            if(new->next != NULL)
                new->next->previous = new;
            new->isFree = true;
            new->size = (node->size - sizeof(Node) - bytes);
            node->size = bytes;
            return node + 1;
        }
        if(node->isFree && node->size >= bytes){
            node->isFree = false;
            return node + 1;
        }
    }
    return NULL;
};

/**
 * Used to free a block of dynamically allocated memory
 * @param memory pointer to start of memory to be deallocated
 */
void deallocate ( void *memory ){
    Node* node = (Node*) memory - 1;
    node->isFree = true;

    if(node->next != NULL && node->next->isFree){
        node->size += node->next->size + sizeof(Node);
        node->next = node->next->next;
        if(node->next != NULL){
            node->next->previous = node;
        }
    }
    if(node->previous != NULL && node->previous->isFree){
        node->previous->size += node->size + sizeof(Node);
        node->previous->next = node->next;
        if(node->next != NULL)
            node->next->previous = node->previous;
    }
};

/**
 *
 */
void printNode(){
    Node* node = first;
    do{

        printf("address %d - next %d - previous %d - isFree %i - size %u \n", node, node->next, node->previous, node->isFree, node->size);
        node = node->next;
    } while(node != NULL);
    printf("end\n");
}





