#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>



int main() {
    unlink("server.sock");
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }

    sockaddr_un address = {};
    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, "server.sock");
    int ret = bind(lfd, (sockaddr*)&address, sizeof(address));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    ret = listen(lfd, 100);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    sockaddr_un clientAddress = {};
    socklen_t len = sizeof(clientAddress);
    int cfd = accept(lfd, (sockaddr*)&clientAddress, &len);
    if(cfd == -1) {
        perror("accept");
        exit(-1);
    }
    printf("client socket filename: %s\n", clientAddress.sun_path);

    while (true) {
        char buf[128];

        int tmpLen = recv(cfd, buf, sizeof(buf), 0);
        if(tmpLen == -1) {
            perror("receive");
            exit(-1);
        } else if(tmpLen == 0) {
            printf("client closed...\n");
            break;
        } else if(tmpLen > 0) {
            printf("receive client data: %s\n", buf);
            send(cfd, buf, sizeof(buf), 0);
        }
    }
    close(lfd);
    return 0;
}