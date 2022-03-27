#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int tickets = 100;
pthread_mutex_t mutex;

void* sellTickets(void *arg) {
    while (tickets > 0) {
        pthread_mutex_lock(&mutex);
        if (tickets > 0) {
            printf("%ld正在卖第%d张票\n", pthread_self(), tickets);
            tickets--;
        } else {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }


    pthread_exit(NULL);
}

int main() {
    pthread_t td1, td2, td3;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&td1, NULL, sellTickets, NULL);
    pthread_create(&td2, NULL, sellTickets, NULL);
    pthread_create(&td3, NULL, sellTickets, NULL);

    pthread_join(td1, NULL);
    pthread_join(td2, NULL);
    pthread_join(td3, NULL);

    pthread_mutex_destroy(&mutex);
}