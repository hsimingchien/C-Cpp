#include <stdio.h>
#include <signal.h>
#include "log.h"

void signalHandler(int sigNo)
{
    if(sigNo == SIGUSR1)
    {
        LOG_DEBUG("Capture SIGUSR1\n");
    }
    else
    {
        // TODO
    }
    LOG_DEBUG("signalHandler running ...\n");
    LOG_DEBUG("The Capture Sig numbers is:%d\n", sigNo);
}

int main(int argc, char* argv[])
{
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);
    while(1)
        sleep(1);
    return 0;
}
