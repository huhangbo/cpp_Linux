#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



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
    fd_set rdSet, tmpSet;
    FD_ZERO(&rdSet);
    FD_SET(lfd, &rdSet);
    int max = lfd;

    while (true) {
        tmpSet = rdSet;
        int selectRet = select(max +1, &tmpSet, NULL, NULL, NULL);
        if(selectRet == -1) {
            perror("select");
            exit(-1);
        } else if(selectRet == 0) {
            continue;
        } else if(selectRet > 0) {
            if(FD_ISSET(lfd, &tmpSet)) {
                sockaddr_in clientAddress = {};
                socklen_t len = sizeof(clientAddress);
                int cfd = accept(lfd, (sockaddr*)&clientAddress, &len);
                if(cfd == -1) {
                    perror("accept");
                    exit(-1);
                }
                FD_SET(cfd, &rdSet);
                max = max > cfd ? max: cfd;
            }
        }

        for (int i = lfd+1; i <= max; ++i) {
            if(FD_ISSET(i, &tmpSet)) {
                char recvBuf[1024];
                ssize_t tmpLen = recv(i, recvBuf, sizeof(recvBuf), 0);
                if(tmpLen == -1) {
                    perror("read");
                    exit(-1);
                } else if(tmpLen > 0) {
                    printf("receive client data: %s\n", recvBuf);
                    send(i, recvBuf, strlen(recvBuf)+1, 0);
                } else if(tmpLen == 0){
                    printf("client closed ...\n");
                    close(i);
                    FD_CLR(i, &rdSet);
                }
            }
        }
    }
    close(lfd);
    return 0;
}