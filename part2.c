//
// Created by Student on 02/12/2019.
//

#include "part2.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


Node *first;
Node *lastVisitedNode;

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
    return node + 1;
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
        return allocateNode(smallestMemoryAddress, bytes);
    }
    if (smallestMemoryAddress->size >= bytes) {
        smallestMemoryAddress->isFree = false;
        return smallestMemoryAddress + 1;
    }
    return NULL;
};

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



void printNode(){
    Node* node = first;
    do{

        printf("address %d - next %d - previous %d - isFree %i - size %u \n", node, node->next, node->previous, node->isFree, node->size);
        node = node->next;
    } while(node != NULL);
    printf("end\n");
}


//allocate all, de allocate B - no merging
static void case1(){
    void* a = allocateBestFit(50);
    void* b = allocateBestFit(50);
    void* c = allocateBestFit(50);
    printNode();
    deallocate(b);
    printNode();

}

//allocate a and b, de allocate b - merge b and remaining space
static void case2(){
    void* a = allocateBestFit(50);
    void* b = allocateBestFit(50);
    printNode();
    deallocate(b);
    printNode();

}

//allocate all, deallocate a & b - a & b merge
static void case3(){
    void* a = allocateBestFit(50);
    void* b = allocateBestFit(50);
    void* c = allocateBestFit(50);
    printNode();
    deallocate(a);
    printNode();
    deallocate(b);
    printNode();

}

//allocate all, deallocate a, c, b - all merge
static void case4(){
    printNode();
    void* a = allocateBestFit(50);
    void* b = allocateBestFit(50);
    void* c = allocateBestFit(50);
    printNode();
    deallocate(a);
    printNode();
    deallocate(c);
    printNode();
    deallocate(b);
    printNode();
}

//allocate all memory - all nodes not free
static void case5(){
    printNode();
    void* a = allocateBestFit(50);
    void* b = allocateBestFit(50);
    void* c = allocateBestFit(50);
    printNode();
    void* d = allocateBestFit(191);
    printNode();
}

//allocate 9
static void bestFitTest10(){
    printNode();
    void* a = allocateBestFit(50);
    void* b = allocateBestFit(20);
    void* c = allocateBestFit(50);
    void* d = allocateBestFit(10);
    void* e = allocateBestFit(50);
    printNode();
    deallocate(b);
    printNode();
    deallocate(d);
    printNode();
    void* f = allocateBestFit(9);
    printNode();
}

//allocate 19
static void bestFitTest20(){
    printNode();
    void* a = allocateBestFit(50);
    void* b = allocateBestFit(10);
    void* c = allocateBestFit(50);
    void* d = allocateBestFit(20);
    void* e = allocateBestFit(50);
    printNode();
    deallocate(a);
    printNode();
    deallocate(d);
    printNode();
    void* f = allocateBestFit(19);
    printNode();
}

static void worstFitTest20(){
    printNode();
    void* a = allocateWorstFit(120);
    void* b = allocateWorstFit(10);
    void* c = allocateWorstFit(50);
    void* d = allocateWorstFit(20);
    void* e = allocateWorstFit(50);
    printNode();
    deallocate(a);
    printNode();
    deallocate(d);
    printNode();
    void* f = allocateWorstFit(19);
    printNode();
}

int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    void* heap = malloc(1500);
    size_t size = 500;
    initialise(heap, size);

    worstFitTest20();

}