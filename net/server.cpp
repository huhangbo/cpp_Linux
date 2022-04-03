#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main() {
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9999);

    int ret = bind(lfd, (sockaddr*)&address, sizeof(address));
    if(ret == -1) {
        perror("bind");
    }

    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
    }
    sockaddr_in clientAddress;
    socklen_t clientAddLen = sizeof(clientAddress);

    int  cfd = accept(lfd, (sockaddr*)&clientAddress, &clientAddLen);
    if(cfd == -1) {
        perror("accept");
    }
    char clientIp[16];
    inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientIp, sizeof(clientIp));
    unsigned short clientPort = ntohs(clientAddress.sin_port);
    printf("client IP: %s, client Port: %d\n", clientIp, clientPort);

    while (true) {
        char recvBuf[1024];
        int len = read(cfd, recvBuf, sizeof(recvBuf));
        if(len == -1) {
            perror("trans\n");
        } else if(len > 0) {
            printf("receive client data: %s\n", recvBuf);
        }

        char* data = "hello, I am server";
        write(cfd, data, strlen(data));
    }
    close(cfd);
    close(lfd);

    return 0;
}
