#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>

#define SERVER_PORT 8888
#define SERVER_IP "192.168.1.101"
#define BACKLOG_MAX 128

#define MAX_BUFF_LEN    1024
typedef unsigned long ulong;

#define MAX_DEBUG_LOOP  10000