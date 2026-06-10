#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t A, B;

void* safe_thread(void* arg) {
    int id = *(int*)arg;

    printf("Thread %d wants resource A\n", id);
    pthread_mutex_lock(&A);
    printf("Thread %d locked resource A\n", id);

    sleep(1);

    printf("Thread %d wants resource B\n", id);
    pthread_mutex_lock(&B);
    printf("Thread %d locked resource B\n", id);

    printf("Thread %d is using both resources safely\n", id);
    sleep(1);

    pthread_mutex_unlock(&B);
    printf("Thread %d released resource B\n", id);

    pthread_mutex_unlock(&A);
    printf("Thread %d released resource A\n", id);

    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    pthread_mutex_init(&A, NULL);
    pthread_mutex_init(&B, NULL);

    pthread_create(&t1, NULL, safe_thread, &id1);
    pthread_create(&t2, NULL, safe_thread, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&A);
    pthread_mutex_destroy(&B);

    return 0;
}