#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define ITER 1000000

int counter = 0;
sem_t sem;

void* increment(void* arg) {
    for (int i = 0; i < ITER; i++) {
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    sem_init(&sem, 0, 1);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem);

    printf("Expected: %d\n", ITER * 2);
    printf("Actual  : %d\n", counter);

    return 0;
}