#pragma once
#include "processData.h"
#include "memory.h"


#define MAX_MEMORY_SIZE 1024

struct algorithm
{
    void* Q;
    struct MemoryNode* memory;
    bool (*addProcess)(struct ProcessData);
    struct ProcessData (*getCurrentProcess)();
    struct ProcessData (*preemptProcess)(struct ProcessData currProcess);
    bool (*removeProcess)();
    bool (*isQueueEmpty)();
};

struct algorithm* newAlgorithm()
{
    struct algorithm* algo = (struct algorithm *)malloc(sizeof(struct algorithm));

    algo->memory = newMemory(0, MAX_MEMORY_SIZE);
    algo->Q = NULL;
    algo->addProcess = NULL;
    algo->getCurrentProcess = NULL;
    algo->preemptProcess = NULL;
    algo->removeProcess = NULL;
    algo->isQueueEmpty = NULL;
    return algo;
}
