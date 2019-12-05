//
// Created by Student on 02/12/2019.
//

#include "part2.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


Node *first;

typedef struct Node {
    struct Node *next;
    struct Node *previous;
    bool isFree;
    size_t size;
} Node;


void initialise(void *memory, size_t size) {
    first = (Node *) memory;
    first->size = size - sizeof(Node);
    first->isFree = true;
    first->next = NULL;
    first->previous = NULL;
};

Node* allocateNode(Node *node, int bytes){
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
    return node;
}

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
        //use allocate node function
        smallestMemoryAddress->isFree = false;
        Node *new = (Node *) ((char *) smallestMemoryAddress + bytes + sizeof(Node));
        new->next = smallestMemoryAddress->next;
        smallestMemoryAddress->next = new;
        new->previous = smallestMemoryAddress;
        if (new->next != NULL)
            new->next->previous = new;
        new->isFree = true;
        new->size = (smallestMemoryAddress->size - sizeof(Node) - bytes);

        smallestMemoryAddress->size = bytes;
        return smallestMemoryAddress + 1;
    }
    if (smallestMemoryAddress->size >= bytes) {
        smallestMemoryAddress->isFree == false;
        return smallestMemoryAddress + 1;
    }
    return NULL;
};

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


int main() {
    void *heap = malloc(sizeof(200));
    size_t size = 200;
    initialise(&heap, size);
    allocateBestFit(200);

}
