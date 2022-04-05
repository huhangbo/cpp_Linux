#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
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
    address.sin_addr.s_addr = INADDR_ANY;

    bind(lfd, (sockaddr*)&address, sizeof(address));

    while (true) {
        char buf[128];
        socklen_t len = sizeof(address);

        recvfrom(lfd, buf, sizeof(buf), 0, (sockaddr *)&address, &len);

        printf("receive data: %s\n", buf);
    }
    close(lfd);
    return 0;
}