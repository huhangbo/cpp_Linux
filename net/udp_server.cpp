#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



int main() {
    int lfd = socket(PF_INET, SOCK_DGRAM, 0);
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

    while (true) {
        char buf[128];
        sockaddr_in clientAddress = {};
        socklen_t len = sizeof(clientAddress);

        recvfrom(lfd, buf, sizeof(buf), 0, (sockaddr *)&clientAddress, &len);

        char clientIP[16];
        inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientIP, sizeof(clientAddress));
        unsigned short clientPort = ntohs(clientAddress.sin_port);
        printf("client IP: %s, client Port: %d\n", clientIP, clientPort);

        printf("receive data: %s\n", buf);

        sendto(lfd, buf, strlen(buf)+1, 0, (sockaddr*)&clientAddress, sizeof(clientAddress));
    }
    close(lfd);
    return 0;
}