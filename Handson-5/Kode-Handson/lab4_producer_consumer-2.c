#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int buffer;
sem_t empty, full;
pthread_mutex_t lock;

void* producer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        sem_wait(&empty);

        pthread_mutex_lock(&lock);
        buffer = i;
        printf("Produced: %d\n", i);
        pthread_mutex_unlock(&lock);

        sem_post(&full);
        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        sem_wait(&full);

        pthread_mutex_lock(&lock);
        printf("Consumed: %d\n", buffer);
        pthread_mutex_unlock(&lock);

        sem_post(&empty);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t p, c;

    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}