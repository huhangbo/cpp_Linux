#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main(void){
    char buf[4096];
    pid_t pid;
    int status;
    printf("%% ");
    while (fgets(buf, 4096, stdin) != nullptr){
        if (buf[strlen(buf)-1] == '\n')
            buf[strlen(buf)-1] = 0;
        if ((pid = fork()) < 0){
            perror("fork error");
        }else if(pid == 0){
            execlp(buf, buf, (char *)0);
            printf("can't execute: %s", buf);
            exit(-1);
        }
        if ((pid = waitpid(pid, &status, 0)) < 0)
            perror("waitpid error");
        printf("%% ");
    }
    return 0;
}