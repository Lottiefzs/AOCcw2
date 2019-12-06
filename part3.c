/*****************************************************************************

 File        : part3.c

 Date        : 5th December 2019

 Description : Implementation of a thread safe memory manager

 Author      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include "part3.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

Node *first;
Node *lastVisitedNode;
void* (*allocate)(size_t bytes);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Node {
    struct Node *next;
    struct Node *previous;
    bool isFree;
    size_t size;
} Node;

typedef struct WorkspaceStruct
{
    unsigned int tid;
}Workspace;

void* tester(void *ws)
{
    Workspace *workspace = (Workspace*)ws;

    void* a = allocate(50);
    printNode();
    printf("Thread: %d", pthread_self());
    void* b = allocate(50);
    printNode();
    printf("Thread: %d", pthread_self());
    deallocate(a);
    printNode();
    printf("Thread: %d", pthread_self());
    return NULL;
}

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

void *allocateBestFit(size_t bytes) {
    pthread_mutex_lock(&mutex);
    Node *smallestMemoryAddress = NULL;
    size_t smallestMemory = SIZE_MAX;

    for (Node *node = first; node != NULL; node = node->next) {
        if (node->isFree && node->size < smallestMemory && node->size >= bytes) {
            smallestMemoryAddress = node;
            smallestMemory = node->size;
        }
    }

    if (smallestMemoryAddress == NULL) {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (smallestMemoryAddress->size >= (bytes + sizeof(Node))) {
        pthread_mutex_unlock(&mutex);
        return allocateNode(smallestMemoryAddress, bytes);
    }
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
    Node *largestMemoryAddress = NULL;
    size_t largestMemory = 0;

    for (Node *node = first; node != NULL; node = node->next) {
        if (node->isFree && node->size > largestMemory && node->size >= bytes) {
            largestMemoryAddress = node;
            largestMemory = node->size;
        }
    }

    if (largestMemoryAddress == NULL) {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (largestMemoryAddress->size >= (bytes + sizeof(Node))) {
        pthread_mutex_unlock(&mutex);
        return allocateNode(largestMemoryAddress, bytes);
    }
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
    Node *node = lastVisitedNode;
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
            pthread_mutex_unlock(&mutex);
            return node + 1;
        }
        if (node->isFree && node->size >= bytes) {
            node->isFree = false;
            lastVisitedNode = node;
            pthread_mutex_unlock(&mutex);
            return node + 1;
        }
        node = node->next;
        if (node == NULL) {
            node = first;
        }
    } while (node != lastVisitedNode);
    pthread_mutex_unlock(&mutex);
    return NULL;
};

void deallocate(void *memory) {
    pthread_mutex_lock(&mutex);
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
    pthread_mutex_unlock(&mutex);
};

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


int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    void *heap = malloc(1500);
    size_t size = 500;
    initialise(heap, size, "bestFit");
    int numberOfThreads = 7;

    pthread_t *threads   = (pthread_t*)calloc(numberOfThreads, sizeof(pthread_t));
    Workspace *workspace = (Workspace*)calloc(numberOfThreads, sizeof(Workspace));

    for (int i = 0; i < numberOfThreads ; ++i) {
        int tid = pthread_create(&(threads[i]), NULL, tester, &(workspace[i]));
        if(tid != 0){
            perror("pthread_create error ");

            return EXIT_FAILURE;
        }
    }

    for ( int i = 0; i < numberOfThreads; i++)
    {
        int tid = pthread_join (threads[i] , NULL );
        if ( tid != 0)
        {
            perror ( "pthread_join error" );
            return EXIT_FAILURE;
        }
        printf ( " tester %d joined \n " , i );
    }
}


