#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

struct sockInfo {
    int fd;
    pthread_t tid;
    sockaddr_in addr;
};

sockInfo sockInfos[128];


void* working(void* arg) {
    sockInfo* pInfo = (sockInfo* )arg;
    char clientIP[16];
    inet_ntop(AF_INET, &pInfo->addr.sin_addr.s_addr, clientIP, sizeof(pInfo->addr));
    unsigned short clientPort = ntohs(pInfo->addr.sin_port);
    printf("client IP: %s, client Port: %d\n", clientIP, clientPort);
    char recvBuf[1024];
    while (true) {
        ssize_t tmpLen = read(pInfo->fd, recvBuf, sizeof(recvBuf));
        if(tmpLen == -1) {
            perror("read");
            exit(-1);
        } else if(tmpLen > 0) {
            printf("receive client data: %s\n", recvBuf);
        } else {
            printf("client closed ...\n");
            break;
        }
        write(pInfo->fd, recvBuf, strlen(recvBuf)+1);
    }
    close(pInfo->fd);
    return NULL;
}

int main() {
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

    int max = sizeof(sockInfos) / sizeof(sockInfo);
    for (int i = 0; i < max; ++i) {
        bzero(&sockInfos[i], sizeof(sockInfo));
        sockInfos[i].fd = -1;
        sockInfos[i].tid = -1;
    }
    while (true) {
        sockaddr_in clientAddress = {};
        socklen_t len = sizeof(clientAddress);
        int cfd = accept(lfd, (sockaddr*)&clientAddress, &len);
        if(cfd == -1) {
            perror("accept");
            exit(-1);
        }
        sockInfo* pInfo;
        for (int i = 0; i < max; ++i) {
            if(sockInfos[i].fd == -1) {
                pInfo = &sockInfos[i];
                break;
            }
            if(i == max-1) {
                sleep(1);
                i = 0;
            }
        }
        pInfo->fd = cfd;
        memcpy(&pInfo->addr, &clientAddress, len);

        int threadRet = pthread_create(&pInfo->tid, NULL, working, pInfo);
        if(threadRet != 0) {
            printf("error: %s\n", strerror(threadRet));
        }
        pthread_detach(pInfo->tid);
    }
    close(lfd);
    return 0;
}