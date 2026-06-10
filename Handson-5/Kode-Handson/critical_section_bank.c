#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 5
#define DEPOSITS_PER_THREAD 3
#define DEPOSIT_AMOUNT 100

int balance = 0;
pthread_mutex_t balance_lock;

void* deposit_money(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 1; i <= DEPOSITS_PER_THREAD; i++) {
        /*
            Critical Section begins here.
            Only one thread should read, modify,
            and write the shared balance at a time.
        */
        pthread_mutex_lock(&balance_lock);

        int old_balance = balance;
        printf("Thread %d reads balance: %d\n", thread_id, old_balance);

        sleep(1);  // Makes the timing easier to observe

        balance = old_balance + DEPOSIT_AMOUNT;
        printf("Thread %d updates balance to: %d\n", thread_id, balance);

        pthread_mutex_unlock(&balance_lock);
        /*
            Critical Section ends here.
        */

        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    int ids[THREADS];

    pthread_mutex_init(&balance_lock, NULL);

    for (int i = 0; i < THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, deposit_money, &ids[i]);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&balance_lock);

    printf("\nFinal balance: %d\n", balance);
    printf("Expected balance: %d\n",
           THREADS * DEPOSITS_PER_THREAD * DEPOSIT_AMOUNT);

    return 0;
}