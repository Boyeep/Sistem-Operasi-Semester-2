#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t forks[N];

void* philosopher(void* num) {
    int i = *(int*)num;

    while (1) {
        printf("Philosopher %d thinking\n", i);
        sleep(1);

        sem_wait(&forks[i]);
        sem_wait(&forks[(i + 1) % N]);

        printf("Philosopher %d eating\n", i);
        sleep(1);

        sem_post(&forks[i]);
        sem_post(&forks[(i + 1) % N]);
    }
}

int main() {
    pthread_t p[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&p[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(p[i], NULL);
    }
}