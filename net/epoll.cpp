#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/epoll.h>


int main() {
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }
    int optVal = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal));

    sockaddr_in address = {};
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(9999);

    int ret = bind(lfd, (sockaddr *) &address, sizeof(address));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    ret = listen(lfd, 128);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    int epollFd = epoll_create(100);   //创建epoll实列
    epoll_event epollEvent = {};
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = lfd;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, lfd, &epollEvent);

    epoll_event epollEvents[1024];

    while (true) {
        int epollRet = epoll_wait(epollFd, epollEvents, 1024, -1);
        if (epollRet == -1) {
            perror("epoll_wait");
            exit(-1);
        }
        printf("epollEvent: %d\n", epollRet);

        for (int i = 0; i < epollRet; ++i) {
            int curFd = epollEvents[i].data.fd;
            if (curFd == lfd) {
                sockaddr_in clientAddress = {};
                socklen_t len = sizeof(clientAddress);
                int cfd = accept(lfd, (sockaddr *) &clientAddress, &len);
                if (cfd == -1) {
                    perror("accept");
                    exit(-1);
                }
                char clientIP[16];
                inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientIP, sizeof(clientAddress));
                unsigned short clientPort = ntohs(clientAddress.sin_port);
                printf("client IP: %s, client Port: %d\n", clientIP, clientPort);

                epollEvent.data.fd = cfd;
                epollEvent.events = EPOLLIN;
                epoll_ctl(epollFd, EPOLL_CTL_ADD, cfd, &epollEvent);
            } else {
                char recvBuf[1024];
                ssize_t tmpLen = recv(curFd, recvBuf, sizeof(recvBuf), 0);
                if (tmpLen == -1) {
                    perror("read");
                    exit(-1);
                } else if (tmpLen > 0) {
                    printf("receive client data: %s\n", recvBuf);
                    send(curFd, recvBuf, strlen(recvBuf) + 1, 0);
                } else if (tmpLen == 0) {
                    printf("client closed ...\n");
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, curFd, nullptr);
                    close(curFd);
                }
            }
        }
    }
    close(epollFd);
    close(lfd);
    return 0;
}