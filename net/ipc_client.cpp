#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>



int main() {
    unlink("client.sock");
    int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(cfd == -1) {
        perror("socket");
        exit(-1);
    }

    sockaddr_un address = {};
    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, "client.sock");
    bind(cfd, (sockaddr*)&address, sizeof(address));

    sockaddr_un serverAddress = {};
    serverAddress.sun_family = AF_LOCAL;
    strcpy(serverAddress.sun_path, "server.sock");

    connect(cfd, (sockaddr*)&serverAddress, sizeof(serverAddress));

    printf("client socket filename: %s\n",serverAddress.sun_path);

    int num = 0;
    while (true) {
        char buf[128];
        sprintf(buf, "hello I am client %d\n", num++);
        send(cfd, buf, sizeof(buf), 0);

        int tmpLen = recv(cfd, buf, sizeof(buf), 0);
        if(tmpLen == -1) {
            perror("receive");
            exit(-1);
        } else if(tmpLen == 0) {
            printf("client closed...\n");
            break;
        } else if(tmpLen > 0) {
            printf("receive server data: %s\n", buf);
        }
        sleep(1);
    }
    close(cfd);
    return 0;
}