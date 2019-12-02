//
// Created by charl on 28/11/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "part1.h"

Node* first;

typedef struct Node{
    struct node * next;
    struct node * previous;
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
    for (Node* node = first; node != NULL ; first = first->next) {
        if(node->isFree == true && node->size >= (bytes + sizeof(Node))){
            node->isFree = false;
            Node* new = (Node*) node + (bytes + sizeof(Node));
            new->next=node->next;
            node->next = new;
            new->previous = node;
            new->isFree = true;
            new->size = (node->size - sizeof(Node) - bytes);
            node->size = bytes;
        }
    }
    return NULL;
};

void deallocate ( void * memory ){

};

int main(){
    void* heap = malloc(sizeof(200));
    size_t size = 200;
    initialise(&heap, size);

}