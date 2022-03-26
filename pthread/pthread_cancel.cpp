#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void* callBack(void* arg) {
    printf("child thread id is %lu\n", pthread_self());
    for(int i = 0; i < 10; i++) {
        printf("child num = %d\n", i);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int num = 10;
    int ret = pthread_create(&tid, NULL, callBack, (void*)&num);
    int* thread_return;
    if(ret != 0) {
        char *errStr = strerror(ret);
        printf("error: %s\n", errStr);
    }

    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    pthread_cancel(tid);

    for (int i = 0; i < 5; ++i) {
        printf("%d\n", i);
    }


    pthread_exit(NULL);
}
