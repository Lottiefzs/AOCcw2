/*****************************************************************************

 File        : part1_test.c

 Date        : 5th December 2019

 Description : Test harness for implementation of a basic memory manager in part1.c

 Author      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

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
    printNode();
    void* d = allocate(191);
    printNode();

}


int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    void* heap = malloc(1500);
    size_t size = 500;
    initialise(heap, size);

    //allocate test First Fit
    allocateTest();

    //deallocate tests
    initialise(heap, size);
    case1();
    initialise(heap, size);
    case2();
    initialise(heap, size);
    case3();
    initialise(heap, size);
    case4;
    initialise(heap, size);
    case5();

}
