#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    sem_t *hei = sem_open("hei", O_CREAT, 0666, 1);
    sem_t *hong = sem_open("hong", O_CREAT, 0666, 0);

    for (int i = 1; i <= 9; i++) {
        sem_wait(hei);
        printf("Step %d: Red moves\n", i);
        fflush(stdout)
        sem_post(hong);
        sleep(rand() % 3);
    }
    printf("Step 10: Red wins\n");
    sem_close(hei);
    sem_close(hong);
    sem_unlink("hei");
    sem_unlink("hong");
    exit(0);
}
