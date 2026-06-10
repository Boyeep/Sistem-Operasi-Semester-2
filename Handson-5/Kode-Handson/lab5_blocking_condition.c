#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int data_ready = 0;
int shared_data = 0;

pthread_mutex_t lock;
pthread_cond_t condition;

void* worker(void* arg) {
    pthread_mutex_lock(&lock);

    while (!data_ready) {
        printf("Worker: data is not ready, blocking now...\n");
        pthread_cond_wait(&condition, &lock);
    }

    printf("Worker: awakened, received shared data = %d\n", shared_data);

    pthread_mutex_unlock(&lock);
    return NULL;
}

void* controller(void* arg) {
    sleep(3);

    pthread_mutex_lock(&lock);

    shared_data = 42;
    data_ready = 1;
    printf("Controller: data prepared, signaling worker...\n");

    pthread_cond_signal(&condition);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t_worker, t_controller;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&condition, NULL);

    pthread_create(&t_worker, NULL, worker, NULL);
    pthread_create(&t_controller, NULL, controller, NULL);

    pthread_join(t_worker, NULL);
    pthread_join(t_controller, NULL);

    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&lock);

    return 0;
}