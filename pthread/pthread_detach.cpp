#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* callBack(void *arg) {
    printf("child thread id is %lu\n", pthread_self());
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
    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    pthread_detach(tid);

    int ret2 = pthread_join(tid, NULL);
    if(ret2 != 0) {
        char *errStr = strerror(ret2);
        printf("error: %s\n", errStr);
    }

    pthread_exit(NULL);
}
