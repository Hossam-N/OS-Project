#pragma once
#include "headers.h"

struct algorithm *initMLF();
bool mlfAddProcess(struct ProcessData prcs);
struct ProcessData mlfGetCurrentProcess();
struct ProcessData mlfPreemptProcess(struct ProcessData currProcess) { return currProcess; }
bool isMLFQueueEmpty();

// Remove process will be end quantum
bool mlfRemoveProcess() { return true; }

void resetMLF();

int currentLevel = 0;
int numOfProcesses = 0;
struct PriQueue *mlfQueue;

struct algorithm *initMLF()
{
    struct algorithm *mlf = newAlgorithm();
    mlfQueue = newPriQueue(12);
    mlf->Q = mlfQueue;
    mlf->addProcess = mlfAddProcess;
    mlf->getCurrentProcess = mlfGetCurrentProcess;
    mlf->preemptProcess = mlfPreemptProcess;
    mlf->removeProcess = mlfRemoveProcess;
    mlf->isQueueEmpty = isMLFQueueEmpty;
    return mlf;
}
bool isMLFQueueEmpty()
{
    if (numOfProcesses == 0)
    {
        return true;
    }
    return false;
}

bool mlfAddProcess(struct ProcessData prcs)
{
    int priority = prcs.priority;

    if (priority < currentLevel)
        priority = currentLevel;

    pushInPriQueue(&mlfQueue[priority], prcs);
    numOfProcesses++;
}

struct ProcessData mlfGetCurrentProcess()
{
    if (numOfProcesses == 0)
    {
        struct ProcessData p;
        p.arrivalTime = -1;
        return p;
    }

    if (currentLevel > 10)
    {
        resetMLF();
        return mlfGetCurrentProcess();
    }
    struct Node *hawhwa = popFromPriQueue(&mlfQueue[currentLevel]);
    if (hawhwa == NULL)
    {
        currentLevel++;
        return mlfGetCurrentProcess();
    }
    numOfProcesses--;
    hawhwa->process.priority++;
    return hawhwa->process;
}

void resetMLF()
{
    struct Node *hawhwa = popFromPriQueue(&mlfQueue[currentLevel]);

    while (hawhwa != NULL)
    {
        hawhwa->process.priority = hawhwa->process.actualPriority;
        pushInPriQueue(&mlfQueue[hawhwa->process.actualPriority], hawhwa->process);
        hawhwa = popFromPriQueue(&mlfQueue[currentLevel]);
    }
    currentLevel = 0;
}