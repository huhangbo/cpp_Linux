#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        char buf[1024] = {0};
        while (1) {
            read(pipefd[0], buf, sizeof(buf));
            printf("parent receive : %s, pid: %d\n", buf, getpid());
        }
    } else if (pid == 0) {
        close(pipefd[0]);
        while (1) {
            char * str = "hello, i am child";
            write(pipefd[1], str, strlen(str));
            sleep(1);
        }
    }
    return 0;
}
