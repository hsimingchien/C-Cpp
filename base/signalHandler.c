#include <stdio.h>
#include <signal.h>

void signalHandler(int sigNo)
{
    if(sigNo == SIGUSR1)
    {
        printf("Capture SIGUSR1\n");
    }
    else
    {
        // TODO
    }
    printf("signalHandler running ...\n");
    printf("The Capture Sig numbers is:%d\n", sigNo);
}

int main(int argc, char* argv[])
{
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);
    while(1)
        sleep(1);
    return 0;
}
