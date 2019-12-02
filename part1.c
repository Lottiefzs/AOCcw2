//
// Created by charl on 28/11/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <vss.h>
#include "part1.h"


typedef struct node{
    struct node * next;
    struct node * previous;
    bool isFree;
    size_t size;
    void * memoryAddress;
}node;

void initialise ( void * memory , size_t size ){

};

void * allocate ( size_t bytes ){

};

void deallocate ( void * memory ){

};

int main(){

}