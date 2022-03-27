#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int tickets = 100;
void* sellTickets(void *arg) {
    while (tickets > 0) {
        usleep(3000);
        printf("%ld正在卖第%d张票\n", pthread_self(), tickets);
        tickets--;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t td1, td2, td3;

    pthread_create(&td1, NULL, sellTickets, NULL);
    pthread_create(&td2, NULL, sellTickets, NULL);
    pthread_create(&td3, NULL, sellTickets, NULL);

    pthread_join(td1, NULL);
    pthread_join(td2, NULL);
    pthread_join(td3, NULL);
}