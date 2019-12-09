/*****************************************************************************
 File        : part1.c

 Date        : 5th December 2019

 Description : Source file for implementation of a basic memory manager, using the
                "First Fit" strategy.

 Authors      : Rebecca Lloyd 100255844 & Charlotte Langton 100250741

 History     : 05/12/2019 - v1.00

 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "part1.h"

//Start of heap
Node* first;
//Size of heap when initialised
size_t heapSize;


void initialise (void *memory , size_t size){
    first = (Node*) memory;
    first->size = size - sizeof(Node);
    first->isFree = true;
    first->next = NULL;
    first->previous = NULL;
    heapSize = size;
};

void* allocate (size_t bytes){
    //Check if heap is initialised
    if(first == NULL){
        fprintf(stdout,"Error: Heap not initialised \n");
        return NULL;
    }
    if(bytes == 0){
        fprintf(stdout,"Error: No memory allocated as requested bytes = %d \n", bytes);
        return NULL;
    }
    for (Node* node = first; node != NULL ; node = node->next) {
        //if the current node is free and the size available in the node is enough for the requested memory
        // and the size of Node + 1
        if(node->isFree && node->size > (bytes + sizeof(Node))){
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
        //if the current node is free and the size available in the node is enough for the requested memory
        // but not for a new node
        if(node->isFree && node->size >= bytes){
            node->isFree = false;
            return node + 1;
        }
    }
    return NULL;
};

void deallocate ( void *memory ){
    //Check that the heap has been initialised
    if(first == NULL){
        fprintf(stdout,"Error: Heap not initialised \n");
        return;
    }

    //Check that the memory given is within the heap
    if((Node*) memory < first || (Node*) memory > first+heapSize){
        fprintf(stdout,"Error: Memory address given outside of heap, cannot deallocate \n");
        return;
    }

    //Get node at the start of the memory
    Node* node = (Node*) memory - 1;

    //Check if node is already free
    if(node->isFree){
        fprintf(stdout,"Error: Node already free \n");
        return;
    }

    node->isFree = true;
    //If node to deallocates' 'next' is not NULL and the node is free
    if(node->next != NULL && node->next->isFree){
        //Set node to deallocates' size to the current size + the next nodes size and the size of a node
        node->size += node->next->size + sizeof(Node);
        //Set node to deallocates next to the next nodes next
        node->next = node->next->next;
        //If nodes new next is != NULL, set next nodes, previous to deallocated node
        if(node->next != NULL){
            node->next->previous = node;
        }
    }
    //If nodes previous != NULL and the node previous to the deallocated node is free
    if(node->previous != NULL && node->previous->isFree){
        node->previous->size += node->size + sizeof(Node);
        node->previous->next = node->next;
        //If nodes new next is != NULL, set next nodes, previous to deallocated node
        if(node->next != NULL)
            node->next->previous = node->previous;
    }
};

void printNode(){
    Node* node = first;
    printf("----------------------------------------------------------\n");
    do{
        printf(" Node address %d - Next %d - Previous %d - isFree %i - Size %u \n", node, node->next,
                node->previous, node->isFree, node->size);
        node = node->next;
    } while(node != NULL);
    printf("----------------------------------------------------------\n");
}





