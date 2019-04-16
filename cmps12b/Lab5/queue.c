///////////////////////////////////////////////////////////////////////
//   Name: Johnson Le
//   Date: 11/16/2017
//   File: queue.c
//   class file for and integer queue
//////////////////////////////////////////////////////////////////////

#include "queue.h"

Node* newNode(int x){
     Node* temp = malloc(sizeof(Node));
     temp->val = x;
     temp->next = NULL;
     return temp;
}

void queueInit(Queue *queue){
     queue->size = 0;
     // queue->head == NULL;
}

void enQueue(Queue *queue, int x){
     Node* temp = newNode(x);
     if(queue->size==0){
          queue->head = temp;
     }
     else{
          temp->next = queue->head;
          queue->head = temp;
     }
     queue->size = queue->size + 1;
}

int deQueue(Queue *queue){
     Node* temp = queue->head;
     if(queue->size==0){
          return -1;
     }
     else if(queue->size==1){
          queue->head = NULL;
     }
     else{
          for(int i=0 ; i<queue->size-1 ; i++){
               temp=temp->next;
          }
     }
     int x = temp->val;
     free(temp);
     queue->size = queue->size - 1;
     return x;
}

void queueFree(Queue *queue){
     Node* temp;
     for(int i=0; i<queue->size ; i++){
          temp = queue->head;
          queue->head = queue->head->next;
          temp->next = NULL;
          free(temp);
     }
     queue->head = NULL;
}

void filePrint(Node* temp, FILE* in, int n){
     // if(temp==NULL){
     //      return;
     // }
     if(n==0){
          return;
     }
     // fprintf(in, "%d ",temp->val);
     n--;
     filePrint(temp->next, in, n);
     fprintf(in, "%d ",temp->val);
}
void printQueue(Queue *queue, FILE* in){
     // Node* temp = queue->head;
     int n = queue->size;
     filePrint(queue->head, in, n);

}
