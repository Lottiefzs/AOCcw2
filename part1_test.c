/*****************************************************************************

 File        : part1_test.c

 Date        : 5th December 2019

 Description : Test harness for implementation of a basic memory manager in part1.c

 Authors      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include "part1.h"
#include <stdlib.h>
#include <stdio.h>

//allocate all
static void allocateTest(){
    Node* a = (Node*) allocate(50) - 1;
    Node* b = (Node*) allocate(50) - 1;
    Node* c = (Node*) allocate(50) - 1;
    printNode();
    if(!a->isFree && !b->isFree && !c->isFree){
        printf("Test passed. All allocated correctly. \n");
    } else {
        printf("Test failed. Memory allocated incorrectly. \n");
    }
}

//allocate all, de allocate B - no merging
static void case1(){
    Node* a = (Node*) allocate(50) - 1;
    Node* b = (Node*) allocate(50) - 1;
    Node* c = (Node*) allocate(50) - 1;
    deallocate(b +1);
    printNode();
    if(!a->isFree && b->isFree && !c->isFree){
        printf("Test passed. All allocated correctly and b deallocated. \n");
    } else {
        printf("Test failed. Memory allocated incorrectly. \n");
    }

}

//allocate a and b, de allocate b - merge b and remaining space
static void case2(){
    Node* a = (Node*) allocate(50) - 1;
    Node* b = (Node*) allocate(50) - 1;
    deallocate(b + 1);
    printNode();
    if(b->next == NULL && b->isFree){
        printf("Test passed. Allocated correctly and b deallocated and merged. \n");
    } else {
        printf("Test failed. B not merged with remaining \n");
    }
}

//allocate all, deallocate a & b - a & b merge
static void case3(){
    Node* a = (Node*) allocate(50) - 1;
    Node* b = (Node*) allocate(50) - 1;
    Node* c = (Node*) allocate(50) - 1;
    deallocate(a + 1);
    deallocate(b + 1);

    if(a->size == 100 + sizeof(Node)){
        printf("Test passed. Allocated correctly and a & b deallocated and merged. \n");
    } else {
        printf("Test failed. A & B not merged \n");
    }
    printNode();

}

//allocate all, deallocate a, c, b - all merge
static void case4(){
    Node* a = (Node*) allocate(50) - 1;
    Node* b = (Node*) allocate(50) - 1;
    Node* c = (Node*) allocate(50) - 1;
    Node* d = (Node*) allocate(50) - 1;
    deallocate(a +1);
    deallocate(c + 1);
    deallocate(b + 1);

    if(a->size == 150 + (sizeof(Node) * 2)){
        printf("Test passed. Allocated correctly and a, b & c deallocated and merged. \n");
    } else {
        printf("Test failed. a, b & c not merged \n");
    }
    printNode();
}

//allocate all memory - all nodes not free
static void case5(int size){
    Node* a = (Node*) allocate(50) - 1;
    Node* b = (Node*) allocate(50) - 1;
    Node* c = (Node*) allocate(50) - 1;
    Node* d = (Node*) allocate(190) - 1;
    printNode();
    int used = a->size + b->size + c->size + d->size;
    if(used + (sizeof(Node) * 4) == size){
        printf("Test passed. Allocated correctly and no memory free \n");
    } else {
        printf("Test failed. \n");
    }

}

static void allocate0bytes(){
    allocate(0);
}

static void deallocateTwice(){
    void* a = allocate(50);
    void* b = allocate(50);
    void* c = allocate(50);
    deallocate(b);
    deallocate(b);
}


int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    void* heap = malloc(1500);
    size_t size = 500;
    printf("----------------------------------------------------------\n");
    //Expected error message as heap not initialised
    printf("Allocate memory when no heap initialised \n");
    if(allocate(100) == NULL){
        printf("Test passed. No memory allocated a heap not initialised. \n");
    } else {
        printf("Test failed. Memory allocated when heap not initialised. \n");
    }

    initialise(heap, size);

    printf("----------------------------------------------------------\n");
    //Expected error message as heap not initialised
    printf("Allocate memory greater than heap size \n");
    if(allocate(10000) == NULL){
        printf("Test passed. No memory allocated as not enough size in heap. \n");
    } else {
        printf("Test failed. Memory allocated when space not available. \n");
    }


    printf("----------------------------------------------------------\n");
    printf("Deallocating memory not in heap should produce - Error: Memory address given outside of heap, cannot deallocate \n");
    deallocate(10);


    printf("----------------------------------------------------------\n");
    printf("Deallocating the same memory twice should produce - Error: Node already free \n");
    deallocateTwice();


    initialise(heap, size);
    printf("----------------------------------------------------------\n");
    printf("Allocate 0 bytes should produce - Error: No memory allocated as requested bytes = 0 \n");
    allocate0bytes();


    printf("----------------------------------------------------------\n");
    printf("Allocate 3 nodes - all 50 bytes \n");
    allocateTest();

    initialise(heap, size);
    printf("----------------------------------------------------------\n");
    printf("Deallocate \n");
    printf("Case 1 - Allocate all, Deallocate B - no merging \n");
    case1();


    initialise(heap, size);
    printf("----------------------------------------------------------\n");
    printf("Case 2 - allocate a and b, de allocate b - merge b and remaining space\n");
    case2();

    initialise(heap, size);
    printf("----------------------------------------------------------\n");
    printf("Case 3 - allocate all, deallocate a & b - a & b merge \n");
    case3();

    initialise(heap, size);
    printf("----------------------------------------------------------\n");
    printf("Case 4 - allocate all, deallocate a, c, b - all merge\n");
    case4();
    initialise(heap, size);
    printf("Case 5 - allocate all memory - no nodes free\n");
    case5(size);

    free(heap);
}
