#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <algorithm>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct node {
    node* next;
    int value;
};

node* head = NULL;

void* customer(void* arg) {
    while(true) {
        pthread_mutex_lock(&mutex);
        if(head != NULL) {
            node* tmp = head;
            head = head->next;
            printf("tid: %ld delete node, num: %d\n", pthread_self(), tmp->value);
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        } else {
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

void* producer(void* arg) {
    while(true) {
        pthread_mutex_lock(&mutex);
        node* newNode = (node*)malloc(sizeof (node));
        newNode->value = rand() % 1000;
        newNode->next = head;
        head = newNode;

        printf("tid: %ld add node, num: %d\n", pthread_self(), newNode->value);

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        usleep(100);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t w[3], r[5];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    for(int i = 0; i < 5; i++) {
        pthread_create(&w[i], NULL, producer, NULL);
        pthread_create(&r[i], NULL, customer, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(w[i]);
        pthread_detach(r[i]);
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}
