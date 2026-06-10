#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5

int buffer[SIZE];
int count = 0;

sem_t empty, full, mutex;

void* producer(void* arg) {
    while (1) {
        sleep(1);  // simulate production time

        sem_wait(&empty);  // wait if buffer is full
        sem_wait(&mutex);  // enter critical section

        buffer[count++] = 1;
        printf("Produced, count = %d\n", count);

        sem_post(&mutex);  // leave critical section
        sem_post(&full);   // signal item available
    }
}

void* consumer(void* arg) {
    while (1) {
        sleep(2);  // simulate consumption time

        sem_wait(&full);   // wait if buffer is empty
        sem_wait(&mutex);  // enter critical section

        buffer[--count] = 0;
        printf("Consumed, count = %d\n", count);

        sem_post(&mutex);  // leave critical section
        sem_post(&empty);  // signal space available
    }
}

int main() {
    pthread_t prod, cons;

    // Initialize semaphores
    sem_init(&empty, 0, SIZE);  // buffer initially empty
    sem_init(&full, 0, 0);      // no items initially
    sem_init(&mutex, 0, 1);     // binary semaphore (mutex)

    // Create threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads (they run infinitely)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Cleanup (not reached in infinite loop, but good practice)
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}