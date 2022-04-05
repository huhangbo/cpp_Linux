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

    int op = 1;
    setsockopt(lfd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(9999);
    inet_pton(AF_INET, "10.0.4.16", &address.sin_addr.s_addr);

    int num = 0;
    while (true) {
        char buf[128];
        sprintf(buf, "hello client... %d", num++);
        sendto(lfd, buf, strlen(buf)+1, 0, (sockaddr*)&address, sizeof(address));

        printf("send data: %s\n", buf);
        sleep(1);
    }
    close(lfd);
    return 0;
}
