#include "socket.h"

bool _stop = false;

void signalHandler(int sigNo);
int init_socket();
int task1(int fd);
int task2(int fd);
int task3(int fd);


int main(int argc, char* argv[]) {
    // 捕捉信号
    signal(SIGUSR1, signalHandler);
    // 屏蔽SIGCHLD信号，由内核处理该信号。SIGCHLD信号由子进程退出时发出.
    // signal(SIGCHLD,SIG_IGN);
    
    int fd = init_socket();
    if(fd < 0) return fd;
    
    if(argc == 1) {
        task1(fd);
    } else {
        switch(*argv[1]) {
            case '1':
                task1(fd);
                break;
            case '2':
                task2(fd);
                break;
            case '3':
                task3(fd);
                break;
            default:
                printf("Input paras error! argc:%d, argv:%c\n", argc, *argv[1]);
        }
    }

    // 6. 关闭套接字
    close(fd);
    return 0;
}

#define MAX_THREADS_NUM 65535
pthread_t tids[MAX_THREADS_NUM];

struct sock_arg {
    uint no;
    int fd;
    struct sockaddr_in *addr;
};

void* th_fn(void *arg);
int task3(int fd) {
    uint i = 0;
    while(!_stop) {
        int c_addr_len = sizeof(struct sockaddr_in);
        struct sockaddr_in *ac_addr = (struct sockaddr_in *)malloc(c_addr_len);
        if (ac_addr == NULL) {
            printf("malloc struct sockaddr_in fail!\n");
            return -1;
        }
        int c_fd = accept(fd, (struct sockaddr*)ac_addr, &c_addr_len);
        if (c_fd == -1) {
            perror("fail to accept!");
            return -2;
        }
        struct sock_arg *arg = (struct sock_arg*)malloc(sizeof(struct sock_arg));
        if (ac_addr == NULL) {
            printf("malloc struct sock_arg fail!\n");
            return -3;
        }
        arg->no = i + 1;
        arg->fd = c_fd;
        arg->addr = ac_addr;
        int err = pthread_create(&tids[i], NULL, th_fn, (void*)arg);
        if (err != 0) {
            perror("fail to create thread!");
            return -err;
        }
        printf("No.%u tid:0x%lx connecting fd:%d!\n", i + 1, (ulong)tids[i], c_fd);
        ++i;
    }
    for(; i > 0; --i) {
        pthread_join(tids[i], NULL);
    }
    return 0;
}

void* th_fn(void *arg) {
    if (arg == NULL) {
        printf("thread recived arg fail!\n");
        return ((void*)1);
    }
    struct sock_arg *sock_arg = (struct sock_arg *)arg;
    int fd = sock_arg->fd;
    struct sockaddr_in *addr = sock_arg->addr;
    int seq = sock_arg->no;
    char ip[32];
    int cnt =0;
    int port = ntohs(addr->sin_port);
    ulong tid = (ulong)pthread_self();
    char buff[MAX_BUFF_LEN];
    inet_ntop(AF_INET, &addr->sin_addr, ip , sizeof(ip));
    while(!_stop) {
        int len = read(fd, buff, sizeof(buff));
        if (len > 0) {
            if(++cnt % 10000 == 0) {
                printf("Thread.%u tid:0x%lx | %s:%d >> recived %d packet.\n", seq, tid, ip, port, cnt);
            }
            memset(buff, 0, sizeof(buff));
            snprintf(buff, sizeof(buff), "%d", port);
            write(fd, buff, strlen(buff) + 1);
        } else {
            if (len == 0) {
                printf("connect close!");
            } else {
                printf("connect fail!");
            }
            break;
        }
    }
    return ((void*)0);
}

int task2(int fd) {
    while(!_stop) sleep(1);
}

int task1(int fd) {
    // 4. 阻塞等待，等待连接到达，连接成功后返回通信用的套接字
    struct sockaddr_in c_addr;
    int c_addr_len = sizeof(c_addr);
    int c_fd = accept(fd, (struct sockaddr*)&c_addr, &c_addr_len);
    if(c_fd == -1) {
        perror("fail to accept!");
        return -4;
    }

    // 5. 开始通信
    char buff[1024];
    char ip[32];
    memset(buff, 0, sizeof(buff));
    memset(ip, 0, sizeof(ip));
    int len = read(c_fd, buff, sizeof(buff));
    if(len > 0) {
        inet_ntop(AF_INET, &c_addr.sin_addr, ip , sizeof(ip));
        printf("%s:%d >> %s\n", ip, ntohs(c_addr.sin_port), buff);
        write(c_fd, buff, len);
    } else if(len == 0) {
        printf("connect close!");
    } else {
        printf("connect fail!");
    }
    close(c_fd);
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
    // 1. 创建监听套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("fail fo socket!");
        return -1;
    }
    printf("create server socket fd[%d] success!\n", fd);

    // 实现port重用
    int reused = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reused, sizeof(int));

    // 2. 绑定监听套接字到本地IP和port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);
    if(-1 == bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        perror("fail to bind!");
        return -2;
    }

    // 3. 设置监听
    if(-1 == listen(fd, BACKLOG_MAX)) {
        perror("fail to listen!");
        return -3;
    }
    printf("-----------------------------------\n");
    return fd;
}