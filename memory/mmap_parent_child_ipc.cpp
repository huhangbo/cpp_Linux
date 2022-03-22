#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int fd = open("test.txt", O_RDWR);
    __off_t size = lseek(fd, 0, SEEK_END);
    void * ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    pid_t pid = fork();
    if(pid > 0) {
        wait();
        char buf[64];
        strcpy(buf, (char *)ptr);
        printf("read data: %s/n", buf);
    } else if(pid == 0) {
        strcpy((char *)ptr, "你好son");
    }
    munmap(ptr, size);
    return 0;
}