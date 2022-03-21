#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main() {
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1) {
        perror("pipe");
        exit(0);
    }
    pid_t pid = fork();

    if (pid > 0) {
        close(pipefd[1]);
        int flags = fcntl(pipefd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(pipefd[0], F_SETFL, flags);
        char buf[1024] = {0};
        while (true) {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len: %d\n", len);
            printf("parent receive : %s, pid: %d\n", buf, getpid());
            memset(buf, 0, 1024);
            sleep(1);
        }
    } else if (pid == 0) {
        close(pipefd[0]);
        while (true) {
            char * str = "hello, i am child";
            write(pipefd[1], str, strlen(str));
            sleep(3);
        }
    }
    return 0;
}
