#pragma once
#include "headers.h"
#include "PriorityQueue.h"


struct algorithm *initHPF();
bool hpfAddProcess( struct ProcessData prcs);
struct ProcessData hpfGetCurrentProcess();
struct ProcessData hpfPreemptProcess(struct ProcessData currProcess);
bool hpfRemoveProcess(){return false;}
bool isHPFQueueEmpty();

struct PriQueue* hpfPriQueue;

struct algorithm *initHPF()
{
    struct algorithm * algo = newAlgorithm();
    hpfPriQueue = newPriQueue(1);
    algo->Q = hpfPriQueue;
    algo->addProcess = hpfAddProcess;
    algo->getCurrentProcess = hpfGetCurrentProcess;
    algo->preemptProcess = hpfPreemptProcess;
    algo->isQueueEmpty = isHPFQueueEmpty;
    algo->removeProcess = hpfRemoveProcess;
    return algo;
}


bool hpfAddProcess(struct ProcessData prcs)
{
    return pushInPriQueue(hpfPriQueue, prcs);
}

bool isHPFQueueEmpty(){
    return isPriQueueEmpty(hpfPriQueue);
}

struct ProcessData hpfGetCurrentProcess()
{
    struct Node* currNode = popFromPriQueue(hpfPriQueue);
    if(currNode == NULL){
        struct ProcessData p;
        p.arrivalTime = -1;
        return p;
    }
    return currNode->process;
}


struct ProcessData hpfPreemptProcess(struct ProcessData currProcess){
    struct Node* currNode = peekFromPriQueue(hpfPriQueue);
    if(currNode){
        if(currNode->process.priority < currProcess.priority){
            kill(currProcess.pid, SIGTSTP);
            hpfAddProcess(currProcess);
            currProcess = hpfGetCurrentProcess();
            kill(currProcess.pid, SIGCONT);
            return currProcess;
        }
    }  
    return currProcess;
}
