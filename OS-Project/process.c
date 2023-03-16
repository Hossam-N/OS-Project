#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int clock;

void continue_handler(int);

int main(int argc, char *argv[])
{

    initClk();
    signal(SIGCONT, continue_handler);
    remainingtime = atoi(argv[1]);

    // TODO The process needs to get the remaining time from somewhere
    // remainingtime = ??;

    clock = getClk();

    while (remainingtime > 0)
    {
        if (getClk() - clock >= 1)
        {
            remainingtime--;
            clock = getClk();
        }
        // remainingtime = ??;
    }

    kill(getppid(), SIGUSR1);
    printf("ana khalst %d at %d \n", getpid(), getClk());
    destroyClk(false);
    return 0;
}

void continue_handler(int signum)
{
    clock = getClk();
    signal(SIGCONT, continue_handler);
}