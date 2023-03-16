#pragma once
#include "headers.h"

FILE *outputLog; // pointer to output file
FILE *memLog;    // pointer to output file
float utilization = 0.0;
int totalRunT = 0;
double avgWTA = 0.0;
int numProc = 0;
int totalWAit = 0;

void printPCB();

typedef short bool;
#define true 1
#define false 0

struct PCB
{
    struct ProcessData *arr;
    bool finish;
    struct ProcessData *finishedProcess;
    int size;
    int maxSize;
};

struct PCB pcb;

void initializePCB(/*int size*/)
{
    outputLog = fopen("output.log", "w"); // opening file
    memLog = fopen("memory.log", "w");    // opening file
    fprintf(outputLog, "#At time x process y state arr w total z remain y wait k\n");
    fflush(outputLog);
    fprintf(memLog, "#At time x allocated bytes for process z from i to j\n");
    fflush(memLog);
    pcb.maxSize = 100;
    pcb.arr = malloc(pcb.maxSize * sizeof(struct ProcessData));
    pcb.size = 0;
    pcb.finish = false;
}

void addProcessPCB(struct ProcessData p)
{

    if (pcb.maxSize == pcb.size)
    {
        pcb.maxSize = pcb.maxSize * 2;
        pcb.arr = realloc(pcb.arr, pcb.maxSize * sizeof(struct ProcessData));
    }
    if (p.state == running)
    {
        p.startingTime = getClk();
    }
    pcb.arr[pcb.size] = p;
    pcb.size++;
}

void removeProcessPCB(int pid)
{
    // for (int i = 0; i < pcb.size; ++i)
    // {
    //     if (pid == pcb.arr[i].pid)
    //     {
    //         pcb.arr[i] = pcb.arr[pcb.size - 1];
    //         pcb.size--;
    //          // calculating no. of finished procceces
    //         break;
    //     }
    // }
}

void printAddedProcess(struct ProcessData p)
{
    fprintf(outputLog, "At time %d process %d arrived total %d remain %d wait %d  \n",
            getClk(), p.inputpid, p.initialRunTime, p.runTime, p.waitingTime);
    fflush(outputLog);

    fprintf(memLog, "At time %d allocated %d bytes for process %d from %d to %d \n",
            getClk(), p.memorySize, p.inputpid, p.memStart, -1+p.memStart+(int)pow(2.0, ceil(log2((double)p.memorySize))));
    fflush(memLog);
}

// printing in log according to states
void printInLog(int i, int state)
{
    // waiting
    int waitingTime = getClk() - pcb.arr[i].arrivalTime - (pcb.arr[i].initialRunTime - pcb.arr[i].runTime);
    if (state == waitting)
    {
        fprintf(outputLog, "At time %d process %d %s arr %d total %d remain %d wait %d\n",
                getClk(), pcb.arr[i].inputpid, "waiting", pcb.arr[i].arrivalTime, pcb.arr[i].initialRunTime, pcb.arr[i].runTime, waitingTime);
        fflush(outputLog);
    }
    // running
    else if (state == running)
    {
        fprintf(outputLog, "At time %d process %d %s arr %d total %d remain %d wait %d\n",
                getClk(), pcb.arr[i].inputpid, "running", pcb.arr[i].arrivalTime, pcb.arr[i].initialRunTime, pcb.arr[i].runTime, waitingTime);
        fflush(outputLog);
    }
    // stopped
    else if (state == stopped)
    {
        fprintf(outputLog, "At time %d process %d %s arr %d total %d remain %d wait %d\n",
                getClk(), pcb.arr[i].inputpid, "stopped", pcb.arr[i].arrivalTime, pcb.arr[i].initialRunTime, pcb.arr[i].runTime, waitingTime);
        fflush(outputLog);
    }
    // finished indeed
    else
    {
        // calculating TA and WTA for finished process
        int TA = getClk() - pcb.arr[i].arrivalTime;
        double WTA = TA / (double)pcb.arr[i].initialRunTime;
        avgWTA += WTA;
        totalWAit += waitingTime; // accumalating total waiting time

        fprintf(outputLog, "At time %d process %d %s arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                getClk(), pcb.arr[i].inputpid, "finished", pcb.arr[i].arrivalTime, pcb.arr[i].initialRunTime, pcb.arr[i].runTime, pcb.arr[i].waitingTime, TA, WTA);
        fflush(outputLog);
        removeProcessPCB(pcb.arr[i].pid);
    }
}

void updatePCB()
{

    for (int i = 0; i < pcb.size; ++i)
    {
        // if (pcb.arr[i].finishTime == -1)
        // {
        //     pcb.arr[i].waitingTime = getClk() - pcb.arr[i].arrivalTime - (pcb.arr[i].initialRunTime - pcb.arr[i].runTime);
        // }
        if (pcb.arr[i].state == waitting || pcb.arr[i].state == stopped)
        {
            pcb.arr[i].waitingTime++;
        }
        else if (pcb.arr[i].state == running)
        {
            if (pcb.arr[i].startingTime == -1)
            {
                pcb.arr[i].startingTime = getClk();
            }
            pcb.arr[i].runTime--;
            totalRunT++;
        }
        else
        {
            pcb.finishedProcess = copyProcess(pcb.arr[i]);
            pcb.finish = true;
            // removeProcessPCB(pcb.arr[i].pid);
        }
        // if(pcb.arr[i].state == finished || pcb.arr[i].state == stopped)
        //  printInLog(i, pcb.arr[i].state);
    }
}

void printPCB()
{

    printf("=========================================UPDATING PCB====================================================\n");
    // if(pcb.finish == true){
    //     printf("At %d, I am process with pid = %d, waiting time = %d, remainning time = %d, startingTime = %d, finishTime = %d\n", getClk()
    //     , pcb.finishedProcess->pid, pcb.finishedProcess->waitingTime, pcb.finishedProcess->runTime, pcb.finishedProcess->startingTime, pcb.finishedProcess->finishTime);
    //     pcb.finish = false;
    //     free(pcb.finishedProcess);
    // }
    for (int i = 0; i < pcb.size; ++i)
    {
        // if(pcb.arr[i].state != finished){
        // if (pcb.arr[i].finishTime == -1 && pcb.arr[i].startingTime != -1)
        // {
        //     printf("At %d, I am process with pid = %d, waiting time = %d, remainning time = %d, startingTime = %d, finishTime = %d\n", getClk(), pcb.arr[i].pid, pcb.arr[i].waitingTime, pcb.arr[i].runTime + 1, pcb.arr[i].startingTime, pcb.arr[i].finishTime);
        // }
        // else
        {
            printf("At %d, I am process with pid = %d, waiting time = %d, remainning time = %d, startingTime = %d, finishTime = %d\n", getClk(), pcb.arr[i].pid, pcb.arr[i].waitingTime, pcb.arr[i].runTime, pcb.arr[i].startingTime, pcb.arr[i].finishTime);
        }
        // }
    }
    printf("====================================================================================================\n");
}

double calcUtil()
{
    double sum = 0;
    for (int i = 0; i < pcb.size; ++i)
    {
        sum += pcb.arr[i].initialRunTime;
    }
    return sum;
}

void updateFinishTime(int pid)
{
    for (int i = 0; i < pcb.size; ++i)
    {
        if (pid == pcb.arr[i].pid)
        {
            printf("initial runtime time = %d\n", pcb.arr[i].initialRunTime);
            pcb.arr[i].finishTime = getClk();
            pcb.arr[i].waitingTime = pcb.arr[i].finishTime - pcb.arr[i].initialRunTime - pcb.arr[i].arrivalTime;
            pcb.arr[i].runTime = 0;
            pcb.arr[i].state = finished;
            printInLog(i, finished);
            fprintf(memLog, "At time %d freed %d bytes for process %d from %d to %d \n",
                    getClk(), pcb.arr[i].memorySize, pcb.arr[i].inputpid, pcb.arr[i].memStart, -1+pcb.arr[i].memStart +(int)pow(2.0, ceil(log2((double)pcb.arr[i].memorySize))));
            fflush(memLog);
            return;
        }
    }
}
void updateProcessStatePCB(int pid, int state)
{

    for (int i = 0; i < pcb.size; ++i)
    {
        if (pid == pcb.arr[i].pid)
        {
            pcb.arr[i].state = state;
            if (state == running && pcb.arr[i].startingTime == -1)
            {
                pcb.arr[i].startingTime = getClk();
            }
            printInLog(i, state);
            return;
        }
    }
}

struct ProcessData getProcess(int pid)
{
    for (int i = 0; i < pcb.size; ++i)
    {
        if (pcb.arr[i].pid == pid)
        {
            return pcb.arr[i];
        }
    }
    struct ProcessData p;
    p.arrivalTime = -1;
    return p;
}
