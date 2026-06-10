#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* slow_task(void* arg) {
    printf("Thread A: waiting for simulated I/O...\n");
    sleep(6);
    printf("Thread A: I/O completed\n");
    return NULL;
}

void* fast_task(void* arg) {
    for (int i = 1; i <= 6; i++) {
        printf("Thread B: working step %d\n", i);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t thread_a, thread_b;

    pthread_create(&thread_a, NULL, slow_task, NULL);
    pthread_create(&thread_b, NULL, fast_task, NULL);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    printf("Both threads finished\n");
    return 0;
}