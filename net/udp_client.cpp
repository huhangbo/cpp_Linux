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
    address.sin_family = AF_INET;
    address.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    int num = 0;
    while (true) {
        char buf[128];
        socklen_t len = sizeof(address);
        sprintf(buf, "hello I am client, data: %d", num++);

        sendto(lfd, buf, strlen(buf)+1, 0, (sockaddr*)&address, sizeof(address));

        recvfrom(lfd, buf, sizeof(buf), 0, (sockaddr *)&address, &len);

        printf("receive data: %s\n", buf);

        sleep(1);
    }
    close(lfd);
    return 0;
}