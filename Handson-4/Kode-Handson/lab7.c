#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* background(void* arg) {
    while (1) {
        printf("[Background thread] Running periodic work...\n");
        sleep(3);
    }

    return NULL;
}

int main() {
    pthread_t bg;

    pthread_create(&bg, NULL, background, NULL);

    for (int i = 1; i <= 10; i++) {
        printf("[Main thread] Working step %d\n", i);
        sleep(1);
    }

    return 0;
}