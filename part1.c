//
// Created by charl on 28/11/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "part1.h"

Node* first;

typedef struct Node{
    struct Node * next;
    struct Node * previous;
    bool isFree;
    size_t size;
}Node;


void initialise ( void * memory , size_t size ){
    first = (Node*) memory;
    first->size = size - sizeof(Node);
    first->isFree = true;
    first->next = NULL;
    first->previous = NULL;
};

void* allocate ( size_t bytes ){
    for (Node* node = first; node != NULL ; node = node->next) {
        if(node->isFree && node->size >= (bytes + sizeof(Node))){
            node->isFree = false;
            Node* new = (Node*) ((char*) node + bytes + sizeof(Node));
            new->next=node->next;
            node->next = new;
            new->previous = node;
            if(new->next != NULL)
                new->next->previous = new;
            new->isFree = true;
            new->size = (node->size - sizeof(Node) - bytes);
            node->size = bytes;
            return node + 1;
        }
        if(node->isFree && node->size >= bytes){
            node->isFree = false;
            return node + 1;
        }
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


    case5();

}

