#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int empty = BUFFER_SIZE;
int full = 0;
sem_t *produce;
sem_t *consume;
int sleep_time=2;
int product_amount=10;

void *producer(void *param) {
    for(int i = 0; i < product_amount; i++) {
        sem_wait(produce);
        int item = rand() % 100;
        buffer[full] = item;
        printf("Producer produced item %d to %d\n", item, full);
        full++;
        empty--;
        fflush(stdout);
        sem_post(consume);
        sleep(rand() % sleep_time);
    }
    pthread_exit(0);
}

void *consumer(void *param) {
    for(int i = 0; i < product_amount; i++) {
        sem_wait(consume);
        full--;
        empty++;
        int item = buffer[full];
        printf("Consumer consumed item %d from %d\n", item, full);
        fflush(stdout);
        sem_post(produce);
        sleep(rand() % sleep_time);
    }
    pthread_exit(0);
}

int main(){
    pthread_t tid1, tid2;
    produce = sem_open("produce", O_CREAT, 0666, BUFFER_SIZE);
    consume = sem_open("consume", O_CREAT, 0666, 0);

    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_close(produce);
    sem_close(consume);
    sem_unlink("produce");
    sem_unlink("consume");

    return 0;
}