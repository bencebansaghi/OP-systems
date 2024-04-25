#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define N 5 // Number of philosophers
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define MAXEAT 7 // Number of times philosophers eat

// States of philosophers
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int eatCount = 0;
int state[N];
sem_t mutex;
sem_t s[N];

void test(int i) {
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        sleep(2);
        printf("Philosopher %d takes chopsticks %d and %d\n", i, LEFT, i);
        printf("Philosopher %d is eating\n", i);
        sem_post(&s[i]);
        eatCount++;
        if (eatCount == MAXEAT) {
            printf("Philosophers ate %d times, stopping the program.\n", MAXEAT);
            for(int i = 0; i < N; i++){
                sem_close(&s[i]);
                sem_unlink("s[i]");
            }
            exit(0);
        }
        
    }
}

void take_chopsticks(int i) {
    sem_wait(&mutex);
    state[i] = HUNGRY;
    printf("Philosopher %d is hungry\n", i);
    test(i);
    sem_post(&mutex);
    sem_wait(&s[i]);
    sleep(1);
}

void put_chopsticks(int i) {
    sem_wait(&mutex);
    state[i] = THINKING;
    printf("Philosopher %d putting chopsticks %d and %d down\n", i, LEFT, i);
    printf("Philosopher %d is thinking\n", i);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void* philosopher(void* num) {
    int i = *(int*)num;
    int think_time;
    int eat_time;

    while (1) {
        think_time = rand() % 10 + 1;
        sleep(think_time);
        take_chopsticks(i);
        eat_time = rand() % 5 + 1;
        sleep(eat_time);
        put_chopsticks(i);
    }
}

int main() {
    printf("There are %d philosophers, and the program continues until they ate %d times.\n", N, MAXEAT);
    int i;
    pthread_t thread[N];
    sem_init(&mutex, 0, 1);

    for (i = 0; i < N; i++)
        sem_init(&s[i], 0, 0);

    for (i = 0; i < N; i++) {
        pthread_create(&thread[i], NULL, philosopher, &i);
        printf("Philosopher %d is thinking\n", i);
    }

    for (i = 0; i < N; i++)
        pthread_join(thread[i], NULL);
}
