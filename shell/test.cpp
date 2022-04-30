#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <wait.h>

int main() {
    pid_t pid = fork();
    if(pid > 0) {  //父进程
        printf("I am parent process, pid: %d\n", getpid());
        int status;
        pid_t c_pid = waitpid(-1,&status,0);
        printf("wait child pid: %d\n",c_pid);

        if(WIFEXITED(status)){   // 正常退出
            printf("child process normal exit. status: %d\n",WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){  // 被信号杀死
            printf("child process killed by signal, signal: %d\n", WTERMSIG(status));
        }
    } else if (pid == 0) {  //子进程
        printf("I am child process, pid: %d\n", getpid());
        execl("hello", nullptr);
    } else if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    exit(0);
}