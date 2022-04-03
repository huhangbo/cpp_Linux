#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>


void recyleChild() {
    while (true) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1 || ret == 0) {
            break;
        } else {
            printf("子进程 %d 被回收了\n", ret);
        }
    }
}


int main() {

    struct sigaction act = {};
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_restorer = recyleChild;
    sigaction(SIGCHLD, &act, NULL);

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }

    sockaddr_in address = {};
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(9999);

    int ret = bind(lfd, (sockaddr*)&address, sizeof(address));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    ret = listen(lfd, 128);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    while (true) {
        sockaddr_in clientAddress = {};
        socklen_t len = sizeof(clientAddress);
        int cfd = accept(lfd, (sockaddr*)&clientAddress, &len);
        if(cfd == -1) {
            if(errno == EINTR) {
                continue;
            }
            perror("accept");
            exit(-1);
        }
        pid_t pid = fork();
        if(pid == 0) {
            char clientIP[16];
            inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientIP, sizeof(clientAddress));
            unsigned short clientPort = ntohs(clientAddress.sin_port);
            printf("client IP: %s, client Port: %d\n", clientIP, clientPort);
            char recvBuf[1024];
            while (true) {
                ssize_t tmpLen = read(cfd, recvBuf, sizeof(recvBuf));
                if(tmpLen == -1) {
                    perror("read");
                    exit(-1);
                } else if(tmpLen > 0) {
                    printf("receive client data: %s\n", recvBuf);
                } else {
                    printf("client closed ...\n");
                    break;
                }
                write(cfd, recvBuf, strlen(recvBuf)+1);
            }
            close(cfd);
            exit(0);
        }
    }
    close(lfd);
}