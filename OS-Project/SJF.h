#pragma once
#include "headers.h"
#include "PriorityQueue.h"
#include "algorithims.h"


struct algorithm *initSJF();
bool sjfAddProcess( struct ProcessData prcs);
struct ProcessData sjfGetCurrentProcess();
struct ProcessData sjfPreemptProcess(struct ProcessData currProcess){return currProcess;}
bool sjfRemoveProcess(){return false;}
bool isSJFQueueEmpty();

// struct algorithm
// {
//     void* sjfPriQueue;
//     bool (*addProcess)(void*, struct ProcessData);
//     struct ProcessData (*getCurrentProcess)(void*);
//     bool (*preemptProcess)(void*);
//     bool (*removeProcess)(void*);
// };

struct PriQueue* sjfPriQueue;

struct algorithm *initSJF()
{
    struct algorithm * algo = newAlgorithm();
    sjfPriQueue = newPriQueue(1);
    algo->Q = sjfPriQueue;
    algo->addProcess = sjfAddProcess;
    algo->getCurrentProcess = sjfGetCurrentProcess;
    algo->preemptProcess = sjfPreemptProcess;
    algo->removeProcess = sjfRemoveProcess;
    algo->isQueueEmpty = isSJFQueueEmpty;
    return algo;
}

bool sjfAddProcess(struct ProcessData prcs)
{
    prcs.priority = prcs.runTime;
    return pushInPriQueue(sjfPriQueue, prcs);
}
bool isSJFQueueEmpty(){
    return isPriQueueEmpty(sjfPriQueue);
}

struct ProcessData sjfGetCurrentProcess()
{
    struct Node* currNode = popFromPriQueue(sjfPriQueue);
    if(currNode == NULL){
        struct ProcessData p;
        p.arrivalTime = -1;
        return p;
    }
    return currNode->process;
}




