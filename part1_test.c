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
    void* a = allocate(50);
    void* b = allocate(50);
    void* c = allocate(50);
    printNode();
}

//allocate all, de allocate B - no merging
static void case1(){
    void* a = allocate(50);
    void* b = allocate(50);
    void* c = allocate(50);
    printNode();
    deallocate(b);
    printNode();

}

//allocate a and b, de allocate b - merge b and remaining space
static void case2(){
    void* a = allocate(50);
    void* b = allocate(50);
    printNode();
    deallocate(b);
    printNode();

}

//allocate all, deallocate a & b - a & b merge
static void case3(){
    void* a = allocate(50);
    void* b = allocate(50);
    void* c = allocate(50);
    printNode();
    deallocate(a);
    printNode();
    deallocate(b);
    printNode();

}

//allocate all, deallocate a, c, b - all merge
static void case4(){
    printNode();
    void* a = allocate(50);
    void* b = allocate(50);
    void* c = allocate(50);
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
    void* a = allocate(50);
    void* b = allocate(50);
    void* c = allocate(50);
    void* d = allocate(191);
    printNode();

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
    printf("Case 2 - allocate a and b, de allocate b - merge b and remaining space\n");
    case2();
    initialise(heap, size);
    printf("Case 3 - allocate all, deallocate a & b - a & b merge \n");
    case3();
    initialise(heap, size);
    printf("Case 4 - allocate all, deallocate a, c, b - all merge\n");
    case4();
    initialise(heap, size);
    printf("Case 5 - allocate all memory - no nodes free\n");
    case5();


}
