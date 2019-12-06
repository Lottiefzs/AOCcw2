/*****************************************************************************

 File        : part2.c

 Date        : 5th December 2019

 Description : Implementation of a memory manager with best fit,
                worst fit and next fit algorithms

 Author      : RRebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include "part2.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


Node *first;
Node *lastVisitedNode;
void* (*allocate)(size_t bytes);

/**
 *
 */
typedef struct Node {
    struct Node *next;
    struct Node *previous;
    bool isFree;
    size_t size;
} Node;

/**
 *
 * @param memory
 * @param size
 * @param algorithm
 */
void initialise(void *memory, size_t size, char *algorithm) {
    first = (Node *) memory;
    first->size = size - sizeof(Node);
    first->isFree = true;
    first->next = NULL;
    first->previous = NULL;
    lastVisitedNode = first;
    if(strcmp(algorithm, "bestFit") == 0){
        allocate = &allocateBestFit;
    }
    if(strcmp(algorithm, "worstFit") == 0){
        allocate = &allocateWorstFit;
    }
    if(strcmp(algorithm, "nextFit") == 0){
        allocate = &allocateNextFit;
    }

};

/**
 *
 * @param node
 * @param bytes
 * @return
 */
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
    return node + 1;
}

/**
 *
 * @param bytes
 * @return
 */
void *allocateBestFit(size_t bytes) {

    Node *smallestMemoryAddress = NULL;
    size_t smallestMemory = SIZE_MAX;

    for (Node *node = first; node != NULL; node = node->next) {
        if (node->isFree && node->size < smallestMemory && node->size >= bytes) {
            smallestMemoryAddress = node;
            smallestMemory = node->size;
        }
    }

    if (smallestMemoryAddress == NULL) {
        return NULL;
    }

    if (smallestMemoryAddress->size >= (bytes + sizeof(Node))) {
        return allocateNode(smallestMemoryAddress, bytes);
    }
    if (smallestMemoryAddress->size >= bytes) {
        smallestMemoryAddress->isFree = false;
        return smallestMemoryAddress + 1;
    }
    return NULL;
};

/**
 *
 * @param bytes
 * @return
 */
void *allocateWorstFit(size_t bytes) {

    Node *largestMemoryAddress = NULL;
    size_t largestMemory = 0;

    for (Node *node = first; node != NULL; node = node->next) {
        if (node->isFree && node->size > largestMemory && node->size >= bytes) {
            largestMemoryAddress = node;
            largestMemory = node->size;
        }
    }

    if (largestMemoryAddress == NULL) {
        return NULL;
    }

    if (largestMemoryAddress->size >= (bytes + sizeof(Node))) {
        return allocateNode(largestMemoryAddress, bytes);
    }
    if (largestMemoryAddress->size >= bytes) {
        largestMemoryAddress->isFree = false;
        return largestMemoryAddress + 1;
    }
    return NULL;
};

/**
 *
 * @param bytes
 * @return
 */
void *allocateNextFit(size_t bytes) {
    Node *node = lastVisitedNode;;
    do {
        if (node->isFree && node->size >= (bytes + sizeof(Node))) {
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
        if (node->isFree && node->size >= bytes) {
            node->isFree = false;
            lastVisitedNode = node;
            return node + 1;
        }
        node = node->next;
        if (node == NULL) {
            node = first;
        }
    } while (node != lastVisitedNode);

    return NULL;
};

/**
 *
 * @param memory
 */
void deallocate(void *memory) {
    Node *node = (Node *) memory - 1;
    node->isFree = true;

    if (node->next != NULL && node->next->isFree) {
        node->size += node->next->size + sizeof(Node);
        node->next = node->next->next;
        if (node->next != NULL) {
            node->next->previous = node;
        }
    }
    if (node->previous != NULL && node->previous->isFree) {
        node->previous->size += node->size + sizeof(Node);
        node->previous->next = node->next;
        if (node->next != NULL)
            node->next->previous = node->previous;
    }
};

/**
 *
 */
void printNode() {
    Node *node = first;
    do {

        printf("address %d - next %d - previous %d - isFree %i - size %u \n", node, node->next, node->previous,
               node->isFree, node->size);
        node = node->next;
    } while (node != NULL);
    printf("end\n");
}

