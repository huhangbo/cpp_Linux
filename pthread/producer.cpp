#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_LEN 10

int buf[BUFFER_LEN] = {0};//缓冲区大小为10,初始化为0
int put = 0;//数据入区位置
int take = 0;//数据出区位置

sem_t product, warehouse;//缓冲区剩余大小
pthread_mutex_t mutex; //互斥量

void* produce(void* arg ) {
    while (true) {
        //模拟正在生产
        int d = 1 + rand () % 100;//代表生产的产品名字

        sem_wait(&product);//P操作
        pthread_mutex_lock(&mutex); //加锁，当一个线程在访问缓冲区时，另一个线程来访问时被阻塞

        buf[put] = d;
        put = (++put) % BUFFER_LEN;
        printf("tid: %lu, produce %d at %d\n",pthread_self(), buf[put], put);

        sem_post(&warehouse);//V操作，可取物品信号量加1，唤醒
        pthread_mutex_unlock(&mutex);
    }
}

void* consume(void* arg) {
    while (true) {

        sem_wait(&warehouse);//P操作,信号量>=0后续代码才能执行，否则阻塞当前进程
        pthread_mutex_lock(&mutex);

        printf("tid: %lu, consume %d at %d\n", pthread_self(), buf[take], take);
        buf[take] = -1;
        take = (++take) % BUFFER_LEN;

        sem_post(&product);//V操作，可生产空位信号量加1
        pthread_mutex_unlock(&mutex);
    }
}

int main () {
    //声明生产者和消费者线程
    pthread_t producer[2], customer[3];

    //初始化信号量和互斥量
    sem_init(&product, 0, 0);
    sem_init(&warehouse, 0, BUFFER_LEN);
    pthread_mutex_init(&mutex, NULL);

    //创建线程
    for(int i = 0; i < 2; i++) {
        pthread_create(&producer[i], NULL, produce, NULL);
    }
    for(int i = 0; i < 3; i++) {
        pthread_create(&customer[i], NULL, consume, NULL);
    }

    //阻塞当前线程，线程执行结束
    for(int i = 0; i < 2; i++) {
        pthread_join(producer[i], NULL);
    }
    for(int i = 0; i < 3; i++) {
        pthread_join(customer[i], NULL);
    }

    //销毁信号量和互斥量
    sem_destroy(&product);
    sem_destroy(&warehouse);
    pthread_mutex_destroy(&mutex);

    return 0;
}