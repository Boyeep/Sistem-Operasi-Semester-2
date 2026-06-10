#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ITEMS 10

int buffer;
sem_t empty;
sem_t full;
pthread_mutex_t lock;

void* producer(void* arg) {
    for (int item = 1; item <= ITEMS; item++) {
        sem_wait(&empty);

        pthread_mutex_lock(&lock);
        buffer = item;
        printf("Producer produced item %d\n", item);
        pthread_mutex_unlock(&lock);

        sem_post(&full);
        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&full);

        pthread_mutex_lock(&lock);
        int item = buffer;
        printf("Consumer consumed item %d\n", item);
        pthread_mutex_unlock(&lock);

        sem_post(&empty);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&lock);
    return 0;
}