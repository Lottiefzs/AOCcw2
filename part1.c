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
            node->isFree == false;
            return node + 1;
        }
    }
    return NULL;
};

void deallocate ( void *memory ){
    Node* node = (Node*) memory - 1;
    node->isFree = true;

    if(node->next != NULL && node->next->isFree){
        node->next = node->next->next;
        if(node->next != NULL)
            node->next->previous = node;
        node->size += node->next->size + sizeof(Node);
    }
    if(node->previous != NULL && node->previous->isFree){
        node->previous->next = node->next;
        if(node->next != NULL)
            node->next->previous = node->previous;
        node->previous->size += node->size + sizeof(Node);
    }

};

int main(){
    void* heap = malloc(sizeof(200));
    size_t size = 200;
    initialise(&heap, size);
    allocate(200);

}