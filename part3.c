/*****************************************************************************

 File        : part3.c

 Date        : 5th December 2019

 Description : Source file for implementation of a thread safe memory manager

 Authors      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include "part3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

//Start of heap
Node* first;
//Size of heap when initialised
size_t heapSize;
//Last visited node for Next Fit allocate function
Node *lastVisitedNode;
//Holds mutex value
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void initialise(void *memory, size_t size, char *algorithm) {
    //Check if algorithm is valid
    if(strcmp(algorithm, "bestFit") == 0){
        allocate = &allocateBestFit;
    } else if (strcmp(algorithm, "worstFit") == 0){
        allocate = &allocateWorstFit;
    } else if (strcmp(algorithm, "nextFit") == 0){
        allocate = &allocateNextFit;
    } else {
        printf("Error: algorithm string given for allocate not valid. Heap not initialised. \n");
        exit(EXIT_FAILURE);
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
    pthread_mutex_lock(&mutex);
    //Check if heap is initialised
    if(first == NULL){
        fprintf(stderr,"Error: Heap not initialised \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    if(bytes == 0){
        fprintf(stderr,"Error: No memory allocated as requested bytes = 0 \n");
        pthread_mutex_unlock(&mutex);
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
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    //If enough space for requested amount + size of node + 1
    if (smallestMemoryAddress->size > (bytes + sizeof(Node))) {
        pthread_mutex_unlock(&mutex);
        return allocateNode(smallestMemoryAddress, bytes);
    }
    //If only space for requested amount
    if (smallestMemoryAddress->size >= bytes) {
        smallestMemoryAddress->isFree = false;
        pthread_mutex_unlock(&mutex);
        return smallestMemoryAddress + 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
};

void *allocateWorstFit(size_t bytes) {
    pthread_mutex_lock(&mutex);
    //Check if heap is initialised
    if(first == NULL){
        fprintf(stderr,"Error: Heap not initialised \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    if(bytes == 0){
        fprintf(stderr,"Error: No memory allocated as requested bytes = 0 \n");
        pthread_mutex_unlock(&mutex);
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
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    //If enough space for requested amount + size of node + 1
    if (largestMemoryAddress->size > (bytes + sizeof(Node))) {
        pthread_mutex_unlock(&mutex);
        return allocateNode(largestMemoryAddress, bytes);
    }

    //If only space for requested amount
    if (largestMemoryAddress->size >= bytes) {
        largestMemoryAddress->isFree = false;
        pthread_mutex_unlock(&mutex);
        return largestMemoryAddress + 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
};

void *allocateNextFit(size_t bytes) {
    pthread_mutex_lock(&mutex);
    //Check if heap is initialised
    if(first == NULL){
        fprintf(stderr, "Error: Heap not initialised \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    if(bytes == 0){
        fprintf(stderr,"Error: No memory allocated as requested bytes = 0 \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    Node *node = lastVisitedNode;

    //Starting at lastVisitedNode, loop until back at lastVisited
    do {
        //If free and enough space for requested amount + size of node + 1
        if (node->isFree && node->size > (bytes + sizeof(Node))) {
            lastVisitedNode = node;
            pthread_mutex_unlock(&mutex);
            return allocateNode(node, bytes);
        }
        //If free and only space for requested amount
        if (node->isFree && node->size >= bytes) {
            node->isFree = false;
            lastVisitedNode = node;
            pthread_mutex_unlock(&mutex);
            return node + 1;
        }
        node = node->next;
        //If at the end of the list, loop back to start
        if (node == NULL) {
            node = first;
        }
    } while (node != lastVisitedNode);
    pthread_mutex_unlock(&mutex);
    return NULL;
};

void deallocate(void *memory) {
    pthread_mutex_lock(&mutex);
    //Check is heap has been initialised
    if(first == NULL){
        fprintf(stdout,"Error: Heap not initialised \n");
        pthread_mutex_unlock(&mutex);
        return;
    }
    //Check if memory address is within heap
    if((Node*) memory < first || (Node*) memory > first+heapSize){
        fprintf(stdout,"Error: Memory given outside of heap, cannot deallocate \n");
        pthread_mutex_unlock(&mutex);
        return;
    }
    //Get node at the start of the memory
    Node *node = (Node *) memory - 1;
    //Check if memory is already free
    if(node->isFree){
        fprintf(stdout,"Error: Node already free \n");
        pthread_mutex_unlock(&mutex);
        return;
    }
    if(node == lastVisitedNode){
        lastVisitedNode = node->next;
    }
    node->isFree = true;
    lastVisitedNode = node;
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
        lastVisitedNode = node->previous;
        node->previous->size += node->size + sizeof(Node);
        node->previous->next = node->next;
        //If nodes new next is != NULL, set next nodes, previous to deallocated node
        if(node->next != NULL)
            node->next->previous = node->previous;
    }
    pthread_mutex_unlock(&mutex);
};

Node *allocateNode(Node *node, int bytes){
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


void printNode() {
    pthread_mutex_lock(&mutex);
    Node *node = first;
    do {

        printf("address %d - next %d - previous %d - isFree %i - size %u \n", node, node->next, node->previous,
               node->isFree, node->size);
        node = node->next;
    } while (node != NULL);
    printf("end\n");
    pthread_mutex_unlock(&mutex);
}





