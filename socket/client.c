#include "socket.h"

#define CONNECT_MAX 128

bool _stop = false;

void signalHandler(int sigNo);
int init_socket();
int task1();
int task2();
int task3();


int main(int argc, char* argv[]) {
    // 捕捉信号
    signal(SIGUSR1, signalHandler);
    
    if(argc == 1) {
        task1();
    } else {
        switch(*argv[1]) {
            case '1':
                task1();
                break;
            case '2':
                task2();
                break;
            case '3':
                task3();
                break;
            default:
                printf("Input paras error!\n");
        }
    }

    return 0;
}

#define MAX_THREADS_NUM 100
pthread_t tids[MAX_THREADS_NUM];

void* th_fn(void *arg);
int task3()
{
    int i = 0;
    int pid = getpid();
    for(; i < MAX_THREADS_NUM; ++i)
    {
        int* no = (int*)malloc(sizeof(int));
        *no = i+1;
        int err = pthread_create(&tids[i], NULL, th_fn, (void*)no);
        if (err != 0) {
            perror("fail to create thread!");
            return -err;
        }
        printf("No.%u tid:0x%lx thread pid:%d!\n", i + 1, (ulong)tids[i], pid);
    }

    for(--i; i > 0; --i) {
        pthread_join(tids[i], NULL);
    }
    return 0;
}

void* th_fn(void *arg) {
    if (arg == NULL) {
        printf("thread recived arg fail!\n");
        return ((void*)1);
    }
    sleep(3);

    int fd = init_socket();
    if(fd < 0)  return ((void*)1);
    
    int no = *(int*)arg;
    char buff[MAX_BUFF_LEN];
    int cnt = 0;
    int len = 0;
    ulong tid = (ulong)pthread_self();
    while(!_stop) {
        memset(buff, 0, sizeof(buff));
        snprintf(buff, sizeof(buff), "Hello, client tid:%x is no.%d test connection with the server!", tid, ++cnt);
        len = send(fd, buff, strlen(buff) + 1, 0);
        memset(buff, 0, sizeof(buff));
        len = recv(fd, buff, len, 0);
        if (len > 0) {
            if(cnt % MAX_DEBUG_LOOP == 0) {
                printf("Thread.%u tid:0x%lx | << recived %d packet, port: %s.\n",
                    no, tid, cnt, buff);
            }
        } else {
            if (len == 0) {
                printf("connect close!\n");
            } else {
                perror("recv fail!\n");
            }
            break;
        }
    }
    close(fd);
    return ((void*)0);
}

int task2() {
    int fds[CONNECT_MAX];
	int i;
	for(i = 0; i <  CONNECT_MAX; ++i) {
		// 1. 创建通信套接字
        fds[i] = init_socket();
		if (fds[i] < 0) {
			break;
		}
		printf("client.%d fd:%d\n", i, fds[i]);
		sleep(0.1);
	}
    
	for(--i; i > 0 && fds[i] > 0; --i) {
		// 4. 关闭套接字
		close(fds[i]);
	}
    return 0;
}

int task1() {
    int fd = init_socket();
    if(fd < 0) return fd;

    // 3. 通信
    char buff[1024];
    sprintf(buff, "Hello, this is a test connection with the server!");
    int len = send(fd, buff, strlen(buff) + 1, 0);
    memset(buff, 0, sizeof(buff));
    len = recv(fd, buff, sizeof(buff), 0);
    if (len > 0) {
        printf(">> %s\n", buff);
    } else if (len == 0) {
        printf("connect close!\n");
    } else {
        perror("recv fail!\n");
    }

    // 4. 关闭套接字
    close(fd);
    return 0;
}

void signalHandler(int sigNo) {
    if(sigNo == SIGUSR1) {
        _stop = true;
    } else {
        // TODO
    }
    printf("The Capture Sig numbers is:%d\n", sigNo);
}

int init_socket() {
    // 1. 创建通信套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("fail fo socket!");
        return -1;
    }
    printf("create client socket fd[%d] success!\n", fd);

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);
    if(-1 == connect(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        close(fd);
        perror("fail to connect!");
        return -2;
    }
    char ip[32];
    memset(ip, 0, sizeof(ip));
    printf("server %s:%d\n", inet_ntop(AF_INET, &addr.sin_addr, ip , sizeof(ip)), ntohs(addr.sin_port));
    printf("-----------------------------------\n");
    return fd;
}