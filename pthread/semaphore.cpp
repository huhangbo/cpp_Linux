#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <algorithm>
#include <semaphore.h>

pthread_mutex_t mutex;
sem_t p_sem, c_sem;

struct node {
    node* next;
    int value;
};

node* head = NULL;

void* customer(void* arg) {
    while(true) {
        sem_wait(&c_sem);
        pthread_mutex_lock(&mutex);
        node* tmp = head;
        head = head->next;
        printf("tid: %ld delete node, num: %d\n", pthread_self(), tmp->value);
        free(tmp);
        pthread_mutex_unlock(&mutex);
        sem_post(&p_sem);
    }
    return NULL;
}

void* producer(void* arg) {
    while(true) {
        sem_wait(&p_sem);
        pthread_mutex_lock(&mutex);

        node* newNode = (node*)malloc(sizeof (node));
        newNode->next = head;
        head = newNode;
        newNode->value = rand() % 1000;
        printf("tid: %ld add node, num: %d\n", pthread_self(), newNode->value);

        pthread_mutex_unlock(&mutex);
        sem_post(&c_sem);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t w[5], r[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&p_sem, 0, 8);
    sem_init(&c_sem, 0, 0);

    for(int i = 0; i < 5; i++) {
        pthread_create(&w[i], NULL, producer, NULL);
        pthread_create(&r[i], NULL, customer, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(w[i]);
        pthread_detach(r[i]);
    }
    sem_destroy(&p_sem);
    sem_destroy(&c_sem);
    pthread_mutex_destroy(&mutex);
    return 0;
}
