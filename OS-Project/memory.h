#pragma once
#include "processData.h"
#include <stdio.h>
#include <math.h>

struct MemoryNode
{
    short isFree;
    struct MemoryNode *left;
    struct MemoryNode *right;
    int memoryStart;
    int size;
};



// This returns a pointer to an empty priority queue
struct MemoryNode *newMemory(int start, int size)
{
    struct MemoryNode *memoryNode = (struct MemoryNode *)malloc(sizeof(struct MemoryNode));
    memoryNode->left = NULL;
    memoryNode->right = NULL;
    memoryNode->memoryStart = start;
    memoryNode->size = size;
    memoryNode->isFree = true;
    return memoryNode;
}

bool allocateMemory(struct MemoryNode *memoryNode, int processSize, int *allocatedMemoryStart)
{
    if(processSize > memoryNode->size)
        return false;
    if (!(processSize <= (memoryNode->size) / 2))
    {
        if (memoryNode->isFree == 1)
        {
            *allocatedMemoryStart = memoryNode->memoryStart;
            memoryNode->isFree = -1;
            return true;
        }
        return false;
    }
    if(memoryNode->isFree == -1)
        return false;
    if (memoryNode->isFree == 1)
    {
        memoryNode->isFree = 0;
        memoryNode->left = newMemory(memoryNode->memoryStart, memoryNode->size / 2);
        memoryNode->right = newMemory(memoryNode->memoryStart + memoryNode->size / 2, memoryNode->size / 2);
        return allocateMemory(memoryNode->left, processSize, allocatedMemoryStart);
    }
    return allocateMemory(memoryNode->left, processSize, allocatedMemoryStart) || allocateMemory(memoryNode->right, processSize, allocatedMemoryStart);
}

bool deallocateMemory(struct MemoryNode *memoryNode, int processSize, int memoryStart)
{
    if (!(processSize <= (memoryNode->size) / 2) && memoryStart == memoryNode->memoryStart)
    {
        memoryNode->isFree = true;
        return true;
    }

    if (memoryStart < memoryNode->memoryStart + (memoryNode->size / 2))
        deallocateMemory(memoryNode->left, processSize, memoryStart);
    else
        deallocateMemory(memoryNode->right, processSize, memoryStart);
    
    if((memoryNode->right->isFree == true )^ (memoryNode->left->isFree == true) == 1)
        memoryNode->isFree = 0;

    if(memoryNode->right->isFree == true && memoryNode->left->isFree == true){
        free(memoryNode->right);
        free(memoryNode->left);
        memoryNode->left = NULL;
        memoryNode->right = NULL;
        memoryNode->isFree = 1;
    }
    
    return false;
}