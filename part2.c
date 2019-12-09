/*****************************************************************************

 File        : part2.c

 Date        : 5th December 2019

 Description : Soure file for implementation of a memory manager with best fit,
                worst fit and next fit algorithms

 Authors      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include "part2.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Start of heap
Node* first;
//Size of heap when initialised
size_t heapSize;
//Last visited node for Next Fit allocate function
Node *lastVisitedNode;
//Function pointer to requested allocate function
void* (*allocate)(size_t bytes);

void initialise(void *memory, size_t size, char *algorithm) {
    //Check algorithm is valid
    if(strcmp(algorithm, "bestFit") == 0){
        allocate = &allocateBestFit;
    } else if (strcmp(algorithm, "worstFit") == 0){
        allocate = &allocateWorstFit;
    } else if (strcmp(algorithm, "nextFit") == 0){
        allocate = &allocateNextFit;
    } else {
        printf("Error: algorithm string given for allocate not valid. Heap not initialised. \n");
        return;
    }
    first = (Node *) memory;
    first->size = size - sizeof(Node);
    first->isFree = true;
    first->next = NULL;
    first->previous = NULL;
    lastVisitedNode = first;
    heapSize = size;
};

void *allocateBestFit(size_t bytes) {
    //Check if heap is initialised
    if(first == NULL){
        fprintf(stdout,"Error: Heap not initialised \n");
        return NULL;
    }
    if(bytes == 0){
        fprintf(stdout,"Error: No memory allocated as requested bytes = 0 \n");
        return NULL;
    }
    Node *smallestMemoryAddress = NULL;
    size_t smallestMemory = SIZE_MAX;

    //Iterate through list to find smallest hole available
    for (Node *node = first; node != NULL; node = node->next) {
        if (node->isFree && node->size < smallestMemory && node->size >= bytes) {
            smallestMemoryAddress = node;
            smallestMemory = node->size;
        }
    }

    //If no free memory return NULL
    if (smallestMemoryAddress == NULL) {
        return NULL;
    }
    //If enough space for requested amount + size of node + 1
    if (smallestMemoryAddress->size > (bytes + sizeof(Node))) {
        return allocateNode(smallestMemoryAddress, bytes);
    }
    //If only space for requested amount
    if (smallestMemoryAddress->size >= bytes) {
        smallestMemoryAddress->isFree = false;
        return smallestMemoryAddress + 1;
    }
    //return NULL;
};

void *allocateWorstFit(size_t bytes) {
    //Check if heap is initialised
    if(first == NULL){
        fprintf(stdout,"Error: Heap not initialised \n");
        return NULL;
    }
    if(bytes == 0){
        fprintf(stdout,"Error: No memory allocated as requested bytes = 0 \n");
        return NULL;
    }

    Node *largestMemoryAddress = NULL;
    size_t largestMemory = 0;

    //Iterate through list to find largest hole available
    for (Node *node = first; node != NULL; node = node->next) {
        if (node->isFree && node->size > largestMemory && node->size >= bytes) {
            largestMemoryAddress = node;
            largestMemory = node->size;
        }
    }

    //If no free memory return NULL
    if (largestMemoryAddress == NULL) {
        return NULL;
    }

    //If enough space for requested amount + size of node + 1
    if (largestMemoryAddress->size > (bytes + sizeof(Node))) {
        return allocateNode(largestMemoryAddress, bytes);
    }

    //If only space for requested amount
    if (largestMemoryAddress->size >= bytes) {
        largestMemoryAddress->isFree = false;
        return largestMemoryAddress + 1;
    }
    //return NULL;
};

void *allocateNextFit(size_t bytes) {
    //Check if heap is initialised
    if(first == NULL){
        fprintf(stdout, "Error: Heap not initialised \n");
        return NULL;
    }
    if(bytes == 0){
        fprintf(stdout,"Error: No memory allocated as requested bytes = 0 \n");
        return NULL;
    }
    Node *node = lastVisitedNode;

    //Starting at lastVisitedNode, loop until back at lastVisited
    do {
        //If free and enough space for requested amount + size of node + 1
        if (node->isFree && node->size > (bytes + sizeof(Node))) {
            lastVisitedNode = node;
            return allocateNode(node, bytes);
        }
        //If free and only space for requested amount
        if (node->isFree && node->size >= bytes) {
            node->isFree = false;
            lastVisitedNode = node;
            return node + 1;
        }
        node = node->next;
        //If at the end of the list, loop back to start
        if (node == NULL) {
            node = first;
        }
    } while (node != lastVisitedNode);
    return NULL;
};

void deallocate(void *memory) {
    //Check is heap has been initialised
    if(first == NULL){
        fprintf(stdout,"Error: Heap not initialised \n");
        return;
    }
    //Check if memory address is within heap
    if((Node*) memory < first || (Node*) memory > first+heapSize){
        fprintf(stdout,"Error: Memory given outside of heap, cannot deallocate \n");
        return;
    }
    //Get node at the start of the memory
    Node *node = (Node *) memory - 1;
    //Check if memory is already free
    if(node->isFree){
        fprintf(stdout,"Error: Node already free \n");
        return;
    }
    node->isFree = true;
    //If node to deallocates' 'next' is not NULL and the node is free
    if(node->next != NULL && node->next->isFree){
        //Set node to deallocates' size to the current size + the next nodes size and the size of a node
        node->size += node->next->size + sizeof(Node);
        //Set node to deallocates next to the next nodes next
        node->next = node->next->next;
        //If nodes new next is != NULL, set next nodes, previous to deallocated node
        if(node->next != NULL){
            node->next->previous = node;
        }
    }
    //If nodes previous != NULL and the node previous to the deallocated node is free
    if(node->previous != NULL && node->previous->isFree){
        node->previous->size += node->size + sizeof(Node);
        node->previous->next = node->next;
        //If nodes new next is != NULL, set next nodes, previous to deallocated node
        if(node->next != NULL)
            node->next->previous = node->previous;
    }
};

Node *allocateNode(Node *node, int bytes) {
    node->isFree = false;
    Node *new = (Node *) ((char *) node + bytes + sizeof(Node));
    new->next = node->next;
    node->next = new;
    new->previous = node;
    if (new->next != NULL)
        new->next->previous = new;
    new->isFree = true;
    new->size = (node->size - sizeof(Node) - bytes);
    node->size = bytes;
    lastVisitedNode = new;
    return node + 1;
}

void printNode(){
    Node* node = first;
    printf("----------------------------------------------------------\n");
    do{
        printf(" Node address %d - Next %d - Previous %d - isFree %i - Size %u \n", node, node->next,
               node->previous, node->isFree, node->size);
        node = node->next;
    } while(node != NULL);
    printf("----------------------------------------------------------\n");
}

