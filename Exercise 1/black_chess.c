#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    sem_t *hei = sem_open("hei", O_CREAT, 0666, 1);
    sem_t *hong = sem_open("hong", O_CREAT, 0666, 0);

    for (int i = 1; i <= 9; i++) {
        sem_wait(hong);
        printf("Step %d: Black moves\n", i);
        fflush(stdout)
        sem_post(hei);
        sleep(rand() % 3);
    }
    printf("Step 10: Black loses\n");
    sem_close(hei);
    sem_close(hong);
    sem_unlink("hei");
    sem_unlink("hong");
    exit(0);
}
