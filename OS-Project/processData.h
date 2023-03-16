#pragma once
#include "headers.h"
typedef short bool;
#define true 1
#define false 0

#define SHKEY 300

struct ProcessData
{
    int pid;
    int priority;
    int arrivalTime;
    int startingTime;
    int runTime;
    int initialRunTime;
    int finishTime;
    int waitingTime;
    int state;
    int actualPriority;
    int inputpid; // added input pid to be displayed in log
    int previousState;
    int memorySize;
    int memStart;
};

enum state{
    waitting = 0,
    running = 1,
    stopped = 2,
    finished = 3
};

 struct Node
 {
     struct ProcessData process;
     struct Node *next;
 };

 

 struct ProcessData *newProcess(int pid, int priority, int arrivalTime)
{
    struct ProcessData *process = (struct ProcessData *)malloc(sizeof(struct ProcessData));
    process->pid = pid;
    process->priority = priority;
    process->arrivalTime = arrivalTime;
    process->waitingTime = 0;
    process->state = waitting;
    process->actualPriority = priority;
    return process;
}

struct ProcessData *copyProcess(struct ProcessData p)
{
    struct ProcessData *process = (struct ProcessData *)malloc(sizeof(struct ProcessData));
    process->arrivalTime = p.arrivalTime;
    process->finishTime = p.finishTime;
    process->pid = p.pid;
    process->priority = p.priority;
    process->runTime = p.runTime;
    process->startingTime = p.startingTime;
    process->state = p.state;
    process->waitingTime = p.waitingTime;
    process->actualPriority = p.actualPriority;
    process->initialRunTime = p.initialRunTime;
    process->memorySize = p.memorySize;
    process->memStart = p.memStart;
    return process;
}

 struct Node *newNode(struct ProcessData process)
 {
     struct Node *node = (struct Node *)malloc(sizeof(struct Node));
     node->next = NULL;
     node->process = process;
     return node;
 }



 bool comparePriority(struct ProcessData p1, struct ProcessData p2){
    return (p1.priority<p2.priority);
 }
