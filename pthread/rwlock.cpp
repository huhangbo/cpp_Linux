#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int num = 10;
pthread_rwlock_t rwlock;

void* read(void* arg) {
    while(true) {
        pthread_rwlock_rdlock(&rwlock);
        printf("==read, tid: %ld, num: %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    pthread_exit(NULL);
}

void* write(void* arg) {
    while(true) {
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("==write, tid: %ld, num: %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t w[3], r[5];
    pthread_rwlock_init(&rwlock, NULL);
    for(int i = 0; i < 3; i++) {
        pthread_create(&w[i], NULL, write, NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&r[i], NULL, read, NULL);
    }

    for(int i = 0; i < 3; i++) {
        pthread_detach(w[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(r[i]);
    }
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
