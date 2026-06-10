#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t A, B;

void* thread1(void* arg) {
    pthread_mutex_lock(&A);
    printf("Thread 1 locked A\n");
    sleep(1);

    pthread_mutex_lock(&B);
    printf("Thread 1 locked B\n");

    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);
    return NULL;
}

void* thread2(void* arg) {
    pthread_mutex_lock(&B);
    printf("Thread 2 locked B\n");
    sleep(1);

    pthread_mutex_lock(&A);
    printf("Thread 2 locked A\n");

    pthread_mutex_unlock(&A);
    pthread_mutex_unlock(&B);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&A, NULL);
    pthread_mutex_init(&B, NULL);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}