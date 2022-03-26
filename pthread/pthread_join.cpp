#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int value = 10;
void* callBack(void* arg) {
    printf("child thread id is %lu\n", pthread_self());
    printf("num = %d\n", *(int*)arg);
    sleep(3);
    pthread_exit((void*)&value);
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
    for (int i = 0; i < 5; ++i) {
        printf("%d\n", i);
    }
    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    pthread_join(tid, (void**)&thread_return);

    printf("回收子线程成功\n返回值: %d\n", *thread_return);

    pthread_exit(NULL);
}
