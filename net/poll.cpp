#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>


int main() {
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }
    int optVal = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal));

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

    pollfd fds[1024];
    for(int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    int max = lfd;

    while (true) {
        int pollRet = poll(fds, max+1, -1);
        if(pollRet == -1) {
            perror("poll");
            exit(-1);
        } else if(pollRet == 0) {
            continue;
        } else if(pollRet > 0) {
            if(fds[0].revents & POLLIN) {
                sockaddr_in clientAddress = {};
                socklen_t len = sizeof(clientAddress);
                int cfd = accept(lfd, (sockaddr*)&clientAddress, &len);
                if(cfd == -1) {
                    perror("accept");
                    exit(-1);
                }
                char clientIP[16];
                inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientIP, sizeof(clientAddress));
                unsigned short clientPort = ntohs(clientAddress.sin_port);
                printf("client IP: %s, client Port: %d\n", clientIP, clientPort);
                for (int i = 1; i < 1024; ++i) {
                    if(fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                max = max > cfd ? max: cfd;
            }
        }

        for (int i = 1; i <= max; ++i) {
            if(fds[i].revents & POLLIN) {
                char recvBuf[1024];
                ssize_t tmpLen = recv(fds[i].fd, recvBuf, sizeof(recvBuf), 0);
                if(tmpLen == -1) {
                    perror("read");
                    exit(-1);
                } else if(tmpLen > 0) {
                    printf("receive client data: %s\n", recvBuf);
                    send(fds[i].fd, recvBuf, strlen(recvBuf)+1, 0);
                } else if(tmpLen == 0){
                    printf("client closed ...\n");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
            }
        }
    }
    close(lfd);
    return 0;
}