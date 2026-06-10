#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* task(void* arg) {
    char* name = (char*) arg;
    printf("Task %s started\n", name);
    sleep(3);
    printf("Task %s finished\n", name);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    printf("Multithread execution\n");
    pthread_create(&t1, NULL, task, "A");
    pthread_create(&t2, NULL, task, "B");
    pthread_create(&t3, NULL, task, "C");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("All threads completed\n");
    return 0;
}