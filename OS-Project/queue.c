// A C program to demonstrate linked list based implementation of queue
#pragma once
#include "processData.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue
{
    struct Node* Head;
    struct Node* Rear;
};

// A utility function to create an empty queue
struct Queue createQueue()
{
    struct Queue q;
    q.Head = NULL;
    q.Rear = NULL;
    return q;
}

// The function to add a key k to q
void enqueue(struct Queue* q, int data)
{
    // Create a new LL node
    struct Node* temp = (struct Node)malloc(sizeof(struct Node));
    temp->data = data;
    temp->next = NULL;
    // If queue is empty, then new node is front and rear both
    if (q->Head == NULL)
    {
        q->Head = temp;
        q->Rear = temp;
        return;
    }

    q->Rear->next = temp;
    q->Rear = q->Rear->next;
}

// Function to remove a key from given queue q
void dequeue(struct Queue q)
{
    // If queue is empty, return NULL.
    if (q->Head == NULL)
        return;

    // Store previous front and move front one node ahead
    struct Node* temp = q->Head;

    q->Head = q->Head->next;

    // If front becomes NULL, then change rear also as NULL

}

void printqueue(struct Queue* q)
{
    struct Node* p = q->Head;
    while (p != NULL)
    {
        printf("process ID:%d ->", p->data.processId);
        p = p->next;
    }
    printf("NULL\n");
}

int isEmpty_Queue(struct Queue* q)
{
    return (q->Head == NULL) ? 1 : 0;
}

process peek_Queue(struct Queue* q)
{
    if (isEmpty_Queue(q))
    {
        process data;
        data.arrivalTime = -1;
        return data;
    }
    return q->Head->data;
}
