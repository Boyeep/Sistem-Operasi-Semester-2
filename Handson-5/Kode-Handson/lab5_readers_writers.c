#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int read_count = 0;
pthread_mutex_t mutex;
pthread_mutex_t write_lock;

void* reader(void* arg) {
    pthread_mutex_lock(&mutex);
    read_count++;

    if (read_count == 1) {
        pthread_mutex_lock(&write_lock);
    }

    pthread_mutex_unlock(&mutex);

    printf("Reader reading\n");
    sleep(1);

    pthread_mutex_lock(&mutex);
    read_count--;

    if (read_count == 0) {
        pthread_mutex_unlock(&write_lock);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* writer(void* arg) {
    pthread_mutex_lock(&write_lock);

    printf("Writer writing\n");
    sleep(2);

    pthread_mutex_unlock(&write_lock);

    return NULL;
}

int main() {
    pthread_t r1, r2, w1;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_lock, NULL);

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w1, NULL);

    return 0;
}