#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>



int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address = {};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9999);

    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    int ret = connect(fd, (sockaddr* )&address, sizeof(address));
    if(ret == -1) {
        perror("connect");
        return -1;
    }

    char sendBuf[1024];
    while (true) {

        fgets(sendBuf, sizeof(sendBuf), stdin);
        write(fd, sendBuf, strlen(sendBuf)+1);

        ssize_t len = read(fd, sendBuf, sizeof(sendBuf));
        if(len == -1) {
            perror("read");
            break;
        } else if(len > 0) {
            printf("rec server data: %s\n", sendBuf);
        } else if(len == 0) {
            printf("server closed...\n");
            break;
        }
        sleep(1);

    }
    close(fd);
    return 0;
}