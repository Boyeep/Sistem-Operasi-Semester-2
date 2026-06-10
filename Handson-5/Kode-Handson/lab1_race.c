#include <stdio.h>
#include <pthread.h>

#define ITER 1000000

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < ITER; i++) {
        counter++;
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected: %d\n", ITER * 2);
    printf("Actual  : %d\n", counter);

    return 0;
}