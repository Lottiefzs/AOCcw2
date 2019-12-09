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
    void *a = allocate(50);
    void *b = allocate(20);
    void *c = allocate(50);
    void *d = allocate(10);
    void *e = allocate(50);
    deallocate(b);
    deallocate(d);
    void *f = allocate(9);
    printNode();
    if(d == f){
        printf("Test passed. 10 byte space allocated to request for 9. \n");
    } else {
        printf("Test failed. Bytes allocated to the wrong node. \n");
    }
}

static void bestFitTest20() {
    void *a = allocate(50);
    void *b = allocate(10);
    void *c = allocate(50);
    void *d = allocate(20);
    void *e = allocate(50);
    deallocate(a);
    deallocate(d);
    void *f = allocate(19);
    printNode();
    if(d == f){
        printf("Test passed. 20 byte space allocated to request for 19. \n");
    } else {
        printf("Test failed. Bytes allocated to the wrong node. \n");
    }
}

static void worstFitTest10() {
    void *a = allocate(10);
    void *b = allocate(10);
    void *c = allocate(50);
    void *d = allocate(120);
    void *e = allocate(50);
    deallocate(a);
    deallocate(d);
    void *f = allocate(9);
    printNode();
    if(d == f){
        printf("Test passed. 120 byte space allocated to request for 9. \n");
    } else {
        printf("Test failed. Bytes allocated to the wrong node. \n");
    }
}

static void worstFitTest20() {
    void *a = allocate(120);
    void *b = allocate(10);
    void *c = allocate(50);
    void *d = allocate(20);
    void *e = allocate(50);
    deallocate(a);
    deallocate(d);
    void *f = allocateWorstFit(19);
    printNode();
    if(a == f){
        printf("Test passed. 120 byte space allocated to request for 19. \n");
    } else {
        printf("Test failed. Bytes allocated to the wrong node. \n");
    }
}

static void nextFitTest() {
    void *a = allocate(10);
    void *b = allocate(100);
    void *c = allocate(50);
    void *d = allocate(20);
    void *e = allocate(50);
    printf("last visited - %d \n", (Node*)e - 1);
    deallocate(b);
    deallocate(d);
    void *g = allocate(97);
    printNode();
        if(g == b){
        printf("Test passed. 97 bytes allocated into 100 byte gap b. \n");
    } else {
        printf("Test failed. Bytes allocated to the wrong node. \n");
    }
}

static void validAlgo(void* heap, size_t size){
    initialise(heap, size, "Not valid");
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    void *heap = malloc(1500);
    size_t size = 500;

    printf("----------------------------------------------------------\n");
    printf("Check algorithm is valid \n");
    validAlgo(heap, size);

    printf("----------------------------------------------------------\n");
    printf("Allocating memory using Best Fit \n");
    initialise(heap, size, "bestFit");
    bestFitTest10();

    printf("----------------------------------------------------------\n");
    initialise(heap, size, "bestFit");
    bestFitTest20();


    printf("----------------------------------------------------------\n");
    printf("Allocating memory using worst Fit \n");
    initialise(heap, size, "worstFit");
    worstFitTest20();

    initialise(heap, size, "worstFit");
    worstFitTest10();

    printf("----------------------------------------------------------\n");
    printf("Allocating memory using next Fit \n");
    initialise(heap, size, "nextFit");
    nextFitTest();

    free(heap);
}
