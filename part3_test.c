/*****************************************************************************

 File        : part3_test.c

 Date        : 5th December 2019

 Description : Test harness for implementation of a thread safe memory manager

 Author      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/
#include "part3.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* worker(void *ws)
{
    void* tid = pthread_self();
    void* a = allocate(5);
    void* b = allocate(5);
    void* c = allocate(5);
    void* d = allocate(5);
    deallocate(b);
    void* e = allocate(5);
    void* f = allocate(2008);
    printf("Test passed - thread %i \n", tid);
    return NULL;
}

void* worker2(void *ws)
{
    void* tid = pthread_self();
    void* a = allocate(5);
    void* b = allocate(5);
    deallocate(a);
    deallocate(b);
    void* c = allocate(5);
    void* d = allocate(5);
    deallocate(b);
    void* e = allocate(5);
    void* f = allocate(2008);
    printf("Test passed - thread %i \n", tid);
    return NULL;
}

void* worker3(void *ws)
{
    void* tid = pthread_self();
    void* a = allocate(5);
    void* b = allocate(5);
    deallocate(a);
    deallocate(b);
    void* c = allocate(5);
    void* d = allocate(5);
    deallocate(b);
    void* e = allocate(5);
    void* f = allocate(70);
    void* g = allocate(70);
    void* h = allocate(70);
    deallocate(f);
    deallocate(e);
    printf("Test passed - thread %i \n", tid);
    return NULL;
}

void threadTest(void* (*worker)(void *ws), char* algorithm){
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    void *heap = malloc(15000);
    size_t size = 5000;
    initialise(heap, size, algorithm);
    int numberOfThreads = 20;

    pthread_t *threads   = (pthread_t*)calloc(numberOfThreads, sizeof(pthread_t));

    for (int i = 0; i < numberOfThreads ; ++i) {
        int tid = pthread_create(&(threads[i]), NULL, worker, NULL);
        if(tid != 0){
            perror("pthread_create error ");

            exit(EXIT_FAILURE);
        }
    }
    for ( int i = 0; i < numberOfThreads; i++)
    {
        int tid = pthread_join (threads[i] , NULL );
        if ( tid != 0)
        {
            perror ( "pthread_join error" );
            exit(EXIT_FAILURE);
        }
    }

    free(heap);
}

int main() {
    printf("TEST - BEST FIT \n");
    threadTest(worker, "bestFit");

    printf("-------------------------------------------------- \n");

    printf("TEST - NEXT FIT \n");
    threadTest(worker2, "nextFit");

    printf("-------------------------------------------------- \n");

    printf("TEST - WORST FIT \n");
    threadTest(worker3, "worstFit");

    printf("-------------------------------------------------- \n");

    printf("TEST - NO FIT \n");
    threadTest(worker2, "no fit");
}
