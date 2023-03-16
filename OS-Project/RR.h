#pragma once
#include "headers.h"

struct algorithm *initRR();
bool rrAddProcess( struct ProcessData prcs);
struct ProcessData rrGetCurrentProcess();
struct ProcessData rrPreemptProcess(struct ProcessData currProcess){return currProcess;}

// Remove process will be end quantum
bool rrRemoveProcess(){return true;}
bool isRRQueueEmpty();


struct PriQueue* rrQueue;

struct algorithm* initRR(){
    struct algorithm* rr = newAlgorithm();
    rrQueue = newPriQueue(1);
    rr->Q = rrQueue;
    rr->addProcess = rrAddProcess;
    rr->getCurrentProcess = rrGetCurrentProcess;
    rr->preemptProcess = rrPreemptProcess;
    rr->removeProcess = rrRemoveProcess;
    rr->isQueueEmpty = isRRQueueEmpty;
    return rr;
}

bool rrAddProcess(struct ProcessData prcs)
{
    // Make all processes have the same priority to turn it into a FIFO queue
    prcs.priority = 0;
    return pushInPriQueue(rrQueue, prcs);
}

bool isRRQueueEmpty(){
    return isPriQueueEmpty(rrQueue);
}
struct ProcessData rrGetCurrentProcess()
{
    struct Node* currNode = popFromPriQueue(rrQueue);
    if(currNode == NULL){
        struct ProcessData p;
        p.arrivalTime = -1;
        return p;
    }
    return currNode->process;
}


