#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void clearResources(int);
int msgq_id;

int main(int argc, char *argv[])
{
	int schAlg = 0;
	int quantum = 1;
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
	FILE* inFile;
	inFile = fopen(argv[1],"r");
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
	for (int i = 2; i < argc; i +=2) {
		if (strcmp(argv[i], "-sch") == 0)
			schAlg = atoi(argv[i+1]);
		else if (strcmp(argv[i], "-q") == 0)
			quantum = atoi(argv[i+1]);
	}
    // 3. Initiate and create the scheduler and clock processes.
	int clockID = fork();
	if (clockID == 0){
		execl("./clk.out"," ", (char*) NULL);
        return 0;
    }

	int schID = fork();
	if (schID == 0){
        char algo[10];
        sprintf(algo, "%d",schAlg);
        char qun[10];
        sprintf(qun, "%d",quantum); 
        printf("%s\t%s\n", algo, qun);
		execl("./scheduler.out", " ", algo, qun, (char*) NULL);
        return 0;
	}
	


    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function. 
    int x = getClk();
    // printf("Current Time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
	
    key_t key_id;
    key_id = ftok("key", 65); //create unique key
    msgq_id = msgget(key_id, 0666 | IPC_CREAT); //create message queue and return id
    // printf("Message Queue ID in pg = %d\n", msgq_id);
    
    // while (!feof(inFile)){
    //     struct ProcessData prcs;
    //     printf("process pid:\n");
    //     char buf[100];
    //     while(buf[0]=='#' && file != NULL)
    //     {
    //         getline(&buf,0,inFile);
    //     }
    //     while()
    //     strtok(&buf,"\t");
    //     sprintf(buf, "%d\t%d\t%d\t%d\n", &prcs.pid, &prcs.arrivalTime, &prcs.runTime, &prcs.priority);
    //     prcs.state = waitting;
    //     prcs.initialRunTime = prcs.runTime;
    //     prcs.waitingTime = 0;
    //     prcs.finishTime = -1;
    //     prcs.startingTime = -1;
    //     prcs.actualPriority = prcs.priority;
    //     prcs.inputpid=prcs.pid; // added input pid to be displayed in log
    //     msgBuf msg;
    //     msg.process = prcs;
    //     msg.mtype = 5;
    //     while (prcs.arrivalTime > getClk());
    //     int send_val = msgsnd(msgq_id, &msg, sizeof(msg.process) , !IPC_NOWAIT);
    //     // printf("%d\t%d\t%d\t%d\t%d\n", prcs.pid, prcs.arrivalTime, prcs.runTime, prcs.priority, getClk());
    //     // TODO: send process to schedule
    
    // }
    char buf[100];
    char*p;
     while(fscanf(inFile, "%s", buf) == 1)  // stops when reaching the end of the file
	{
        
		if (buf[0] == '#')
		{
			fscanf(inFile,"%*[^\n]\n");   // skips the line (till \n)
		}
		else
		{
            struct ProcessData prcs;
			prcs.pid = (int) strtol(buf, &p, 10); // converts first string into integer
			fscanf(inFile, "%d", &prcs.arrivalTime);
			//printf("arrival: %d\n", arrival);
			fscanf(inFile, "%d", &prcs.initialRunTime);
			prcs.runTime=prcs.initialRunTime;
            fscanf(inFile, "%d", &prcs.priority);
            fscanf(inFile, "%d", &prcs.memorySize);
            prcs.waitingTime = 0;
            prcs.finishTime = -1;
            prcs.startingTime = -1;
            prcs.actualPriority = prcs.priority;
            prcs.inputpid=prcs.pid; // added input pid to be displayed in log
            msgBuf msg;
            msg.process = prcs;
            msg.mtype = 5;
            while (prcs.arrivalTime > getClk());
            int send_val = msgsnd(msgq_id, &msg, sizeof(msg.process) , !IPC_NOWAIT);
            
        }
   }
    kill(schID, SIGUSR2);
   
    // printf("Current Time is %d\n", getClk());

	// TODO: pass current proces to the scheduler and pass all functions with the same time tpp
	
    // sleep(15);
    int statlock;
    wait(&statlock);
	
	msgctl(msgq_id,IPC_RMID,NULL);
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    msgctl(msgq_id,IPC_RMID,NULL);
    destroyClk(true);
    exit(0);
}
