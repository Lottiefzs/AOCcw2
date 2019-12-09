/*****************************************************************************

 File        : part1_test.c

 Date        : 5th December 2019

 Description : Test harness for implementation of a memory manager with best fit,
                worst fit and next fit algorithms in part2.c

 Author      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include "part2.h"
#include <stdlib.h>
#include <stdio.h>

//allocate 9
static void bestFitTest10() {
    void *a = allocateBestFit(50);
    void *b = allocateBestFit(20);
    void *c = allocateBestFit(50);
    void *d = allocateBestFit(10);
    void *e = allocateBestFit(50);
    printNode();
    deallocate(b);
    deallocate(d);
    printNode();
    void *f = allocateBestFit(9);
    printNode();
}

//allocate 19
static void bestFitTest20() {
    void *a = allocateBestFit(50);
    void *b = allocateBestFit(10);
    void *c = allocateBestFit(50);
    void *d = allocateBestFit(20);
    void *e = allocateBestFit(50);
    printNode();
    deallocate(a);
    printNode();
    deallocate(d);
    printNode();
    void *f = allocateBestFit(19);
    printNode();
}

static void worstFitTest20() {
    printNode();
    void *a = allocateWorstFit(120);
    void *b = allocateWorstFit(10);
    void *c = allocateWorstFit(50);
    void *d = allocateWorstFit(20);
    void *e = allocateWorstFit(50);
    printNode();
    deallocate(a);
    printNode();
    deallocate(d);
    printNode();
    void *f = allocateWorstFit(19);
    printNode();
}

static void nextFitTest() {
    void *a = allocateNextFit(10);
    void *b = allocateNextFit(30);
    void *c = allocateNextFit(50);
    void *d = allocateNextFit(20);
    void *e = allocateNextFit(50);
    printNode();
    deallocate(a);
    deallocate(d);
    printNode();
    void *g = allocateNextFit(97);
    printNode();
    void *h = allocateNextFit(6);
    printNode();
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    void *heap = malloc(1500);
    size_t size = 500;

    printf("----------------------------------------------------------\n");
    printf("Allocating memory using Best Fit \n");
    initialise(heap, size, "bestFit");
    printf("Memory of 10 should be allocated in position 4\n");
    bestFitTest10();
    printf("Memory of 20 should be allocated in position 4\n");
    initialise(heap, size, "bestFit");
    bestFitTest20();

    printf("----------------------------------------------------------\n");
    printf("Allocating memory using Next Fit \n");
    initialise(heap, size, "nextFit");
    nextFitTest();
    printf("----------------------------------------------------------\n");
    printf("Allocating memory using Worst Fit \n");
    initialise(heap, size, "worstFit");
    worstFitTest20();

}
