///////////////////////////////////////////////////////////////////////
//   Name: Johnson Le
//   Date: 11/16/2017
//   File: queue.h
//   Header file for and integer queue
//////////////////////////////////////////////////////////////////////

#ifndef _INTEGER_QUEUE_H_
#define _INTEGER_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef int bool;

typedef struct Node{
     int val;
     struct Node* next;
}Node;

typedef struct Queue{
     int size;
     struct Node* head;
}Queue;

Node* newNode(int x);
void queueInit(Queue *queue);
void enQueue(Queue *queue, int x);
int deQueue(Queue *queue);
void queueFree(Queue *queue);
void printQueue(Queue *queue, FILE* in);
void filePrint(Node* temp, FILE* in, int n);

#endif
