#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* worker(void* arg) {
    int id = *(int*)arg;

    for (int i = 1; i <= 5; i++) {
        printf("Thread %d is running: step %d\n", id, i);
        usleep(100000);
    }

    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have finished.\n");
    return 0;
}