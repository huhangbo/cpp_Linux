#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>



int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9999);

    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    int ret = connect(fd, (sockaddr* )&address, sizeof(address));
    if(ret == -1) {
        perror("connect");
        return -1;
    }
    while (true) {
        char* data = "hello, I am client";
        write(fd, data, strlen(data));

        char recvBuf[1024] = {0};
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if(len == -1) {
            perror("read");
            return -1;
        } else if(len > 0) {
            printf("rec server data: %s\n", recvBuf);
        } else if(len == 0) {
            printf("server closed...\n");
        }
        sleep(1);
    }

    close(fd);
    return 0;
}