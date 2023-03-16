#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processData.h"

// struct Node
// {
//     struct ProcessData *processData;
//     struct Node *next;
// };

struct circQueue
{
   struct Node *front ,*rear;
    
};

struct circQueue *CreateCircQueue()
{
    struct circQueue *queue = (struct circQueue *)malloc(sizeof(struct circQueue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

int isEmpty(struct circQueue *queue)
{
    return queue->front == NULL;
}

void push_q(struct circQueue *queue, struct ProcessData *nProcess)
{
    struct Node *temp = newNode(nProcess);
  
    if (isEmpty(queue))
    {
        queue->front = temp;
        queue->rear = temp;
        queue->rear->next=queue->front;
    }
    else
    {
        queue->rear->next = temp;
        queue->rear = temp;
        queue->rear->next=queue->front;
    }
}

void pop_q(struct circQueue *queue)
{
    if (isEmpty(queue))
    {
        printf("Circular Queue is empty!\n");
        return;
    }

    //check if last element is to be popped
    if(queue->front==queue->rear)
    {
        free(queue->front);
        queue->front=NULL;
        queue->rear=NULL;
    }
    else{
        struct Node *temp = queue->front;
        queue->front = queue->front->next;
        queue->rear->next=queue->front;
        free(temp);
    }
    
    //just to be sure :)
    if (queue->front == NULL)
    {
        queue->rear= NULL;
    }

    
}

//Done: queue peak() function // depends on process data structure
struct ProcessData* peak(struct circQueue *queue){

    if(isEmpty(queue))
    {
        printf("Circular Queue is empty!\n");
        return NULL;
    }
    return queue->front->process;
}

void displayCircQueue(struct circQueue* queue)
{
    struct Node* temp = queue->front;
    if(queue->front==NULL)
    {
        printf("Circular Queue is empty!\n");
        return;
    }
    printf("\nElements in Circular Queue are: ");
    while (temp->next != queue->front) {
        printf("%d ",temp->process->pid);
        temp = temp->next;
    }
    printf("%d \n",temp->process->pid);
}