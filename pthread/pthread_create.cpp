#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* callBack(void *arg) {
    printf("child thread...\n");
    printf("num = %d\n", *(int*)arg);
    return NULL;
}

int main() {
    pthread_t tid;
    int num = 10;
    int ret = pthread_create(&tid, NULL, callBack, (void*)&num);
    if(ret != 0) {
        char *errStr = strerror(ret);
        printf("error: %s\n", errStr);
    }
    for (int i = 0; i < 5; ++i) {
        printf("%d\n", i);
    }
    sleep(1);
    return 0;
}
