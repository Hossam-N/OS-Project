#include "headers.h"
#include <stdio.h>
#include "pcb.h"

FILE* outstat;


int algo;
int msgq_id;
struct ProcessData* runningProcess = NULL;
int quantum;
int quantumCounter;


struct algorithm *algoData;
void checkMessageReceived();
struct ProcessData  addProcess(struct ProcessData process);
void sigUserHandler(int);
void sigUser2Handler(int);
void initilization();
bool isProcessGeneratorFinished = false;

struct PriQueue virtualMemoryHardDisc;

int main(int argc, char *argv[])
{

    printf("Haw haw haw\n");
    isProcessGeneratorFinished = false;
    initClk();
    algo = atoi(argv[1]);
    quantum = atoi(argv[2]);

    signal(SIGUSR1, sigUserHandler);
    signal(SIGUSR2, sigUser2Handler);
    key_t key_id;
    initilization();
    key_id = ftok("key", 65);                   // create unique key
    msgq_id = msgget(key_id, 0666 | IPC_CREAT); // create message queue and return id
    // printf("Message Queue ID in pg = %d\n", msgq_id);
    initializePCB();
    // TODO: implement the scheduler.
    int clk = 0;
    printf("CLK NOW = %d \n", clk);
    quantumCounter = 0;
    while (1)
    {
        // printf(" %d %d %d \n", isProcessGeneratorFinished, isProcessGeneratorFinished, runningProcess == NULL);
        
        checkMessageReceived();
        if (getClk() - clk == 1)
        {
            printf("CLK NOW = %d \n", clk+1);
            
            if (quantumCounter >= quantum)
            {
                quantumCounter = 0;
                
                if (algoData->removeProcess()){
                    if (runningProcess != NULL)
                    {
                        kill(runningProcess->pid, SIGTSTP);
                        runningProcess->state = stopped;
                        updateProcessStatePCB(runningProcess->pid, stopped);
                        algoData->addProcess(*runningProcess);

                        // struct ProcessData tmp = algoData->getCurrentProcess();
                        // free(runningProcess);
                        // runningProcess = copyProcess(tmp);
                        *runningProcess = algoData->getCurrentProcess();
                        if (runningProcess->arrivalTime != -1)
                        {
                            runningProcess->state = running;
                            updateProcessStatePCB(runningProcess->pid, running);
                            kill(runningProcess->pid, SIGCONT);
                        }
                        // runningProcess->state = running;
                        // updateProcessStatePCB(runningProcess->pid, running);
                        // kill(runningProcess->pid, SIGCONT);
                    }
                }
            }
            quantumCounter++;
            updatePCB();
            printPCB();
            clk = getClk();
            if(isProcessGeneratorFinished == true && algoData->isQueueEmpty() == true && runningProcess == NULL)
                break;
        }

    }
    printf("Scheduler out of main loop and now terminating \n");
    double clock=getClk()-1.0f;
    utilization=(calcUtil()/clock)*100.0f;
    avgWTA=avgWTA/(double)pcb.size;
    double avgWait=totalWAit/(double)pcb.size;
    outstat = fopen("schedular.perf", "w"); // opening file
    fprintf(outstat, "CPU utilization = %.2f \n",utilization);
    fflush(outstat);
    fprintf(outstat, "Avg WTA = %.2f\n",avgWTA);
    fflush(outstat);
    fprintf(outstat, "Avg Waiting = %.2f\n",avgWait);
    fflush(outstat);



    // TODO: upon termination release the clock resources.
	// msgctl(msgq_id,IPC_RMID,NULL);
    destroyClk(true);
    return 0;
}

void forkProcess(struct ProcessData process){
    printAddedProcess(process);

    int pid = fork();
    if (pid == 0)
    {
        char pRemainingTime[10];
        sprintf(pRemainingTime, "%d", process.runTime);
        execl("./process.out", "", pRemainingTime, (char *)NULL);
        exit(0);
    }
    // sleep(0.1);
    // printf("MESSAGE RECEIVED with pid = %d \n", msg.process.pid);
    process.pid = pid;
    addProcessPCB(process);
    if (runningProcess != NULL)
    {
        updateProcessStatePCB( process.pid, waitting);
        process.state = waitting;
        kill(pid, SIGTSTP);

        process = addProcess(process);
    }
    else
    {
        updateProcessStatePCB(process.pid, running);
        process.state = running;
        runningProcess = copyProcess(process);
    }
}

void checkMessageReceived()
{
    msgBuf msg;
    int rec_val = msgrcv(msgq_id, &msg, sizeof(msg.process), 0, IPC_NOWAIT);
    if (rec_val != -1)
    {
        
        if(!allocateMemory(algoData->memory, msg.process.memorySize, &msg.process.memStart)){
            pushInPriQueue(&virtualMemoryHardDisc, msg.process);
            return;
        }
        forkProcess(msg.process);
    }
}

void sigUserHandler(int signum)
{
    deallocateMemory(algoData->memory, runningProcess->memorySize, runningProcess->memStart);
   
    updateFinishTime(runningProcess->pid);
    struct ProcessData tmp = algoData->getCurrentProcess();
    free(runningProcess);
    runningProcess = copyProcess(tmp);

    quantumCounter = 0;

    if (runningProcess->arrivalTime != -1)
    {
        updateProcessStatePCB(runningProcess->pid, running);
        kill(runningProcess->pid, SIGCONT);
    }else{
        runningProcess = NULL;
    }

    while(!isPriQueueEmpty(&virtualMemoryHardDisc)){
        struct Node *nextNode= peekFromPriQueue(&virtualMemoryHardDisc);
        if(nextNode != NULL){
            struct ProcessData p = nextNode->process;
            if(!allocateMemory(algoData->memory,p.memorySize, &p.memStart))
                break;
            popFromPriQueue(&virtualMemoryHardDisc);
            forkProcess(p);
        }
    }
    signal(SIGUSR1, sigUserHandler);
}

void sigUser2Handler(int signum)
{
    isProcessGeneratorFinished = true;
    signal(SIGUSR2, sigUser2Handler);
}

struct ProcessData addProcess(struct ProcessData process)
{
    algoData->addProcess(process);
    // printf("ANA HENA awel a dkhlt running process %d and process el fat %d \n", runningProcess->pid, process.pid);

    
    struct ProcessData prcs = algoData->preemptProcess(*runningProcess);
    if (prcs.pid==runningProcess->pid)
    {
        return process;
    }
    updateProcessStatePCB(runningProcess->pid, stopped);
    kill(runningProcess->pid, SIGTSTP);
    *runningProcess = prcs;
    updateProcessStatePCB(runningProcess->pid, running);
    kill(runningProcess->pid, SIGCONT);
    // printf("ANA HENA abl ma akhrog running process %d and process el fat %d \n", runningProcess->pid, process.pid);

    return process;
}



void initilization()
{
    switch (algo)
    {
    case SJF:
        algoData = initSJF();
        break;

    case HPF:
        algoData = initHPF();
        break;
    case RR:
        algoData = initRR();
        break;
    case MLF:
        algoData = initMLF();
        break;
    default:
        break;
    };
}
