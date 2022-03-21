#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1) {
        perror("pipe");
        exit(0);
    }
    pid_t pid = fork();

    if(pid > 0) {
        close(fd[1]);
        char buf[1024] = {0};
        read(fd[0], buf, sizeof(buf)-1);
        printf("%s", buf);
    } else if(pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDERR_FILENO);
        execlp("ps", "ps", "aux", NULL);
    } else {
        perror("fork");
        exit(0);
    }
    return 0;
}