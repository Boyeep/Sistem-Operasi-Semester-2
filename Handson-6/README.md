# Hands-On 6

This file is a cleaned OCR transcription of the screenshots in `Handson-6`.

## Lab 1 - Thread Concurrency and Resource Competition

### Objective, Motivation, and Main Concept

The first step in understanding concurrency is recognizing that multiple threads can execute independently while sharing system resources. In a concurrent system, processes or threads may execute in an interleaved manner, leading to unpredictable execution ordering. This behavior becomes especially critical when threads compete for limited resources. The objective of this lab is to introduce students to concurrent thread execution and the fundamental concept of resource competition without yet enforcing synchronization.

From a real-world perspective, nearly all computing systems, from web servers to mobile applications, rely on concurrent execution. For example, multiple users accessing a server simultaneously result in threads competing for CPU time, memory, and I/O devices. Understanding how threads behave under competition is essential to designing efficient and correct systems.

According to Chapter 6, concurrency issues arise because processes may compete for reusable resources, such as memory, files, or devices. When multiple threads attempt to access such resources simultaneously without coordination, the system behavior becomes nondeterministic. This is the foundation upon which more severe issues like deadlock and starvation are built.

This lab demonstrates that even without explicit errors, concurrent execution alone introduces unpredictability. Students will observe how thread interleaving occurs and how shared resources become points of contention.

### Implementation (C with POSIX Threads)

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* task(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < 5; i++) {
        printf("Thread %d is running iteration %d\n", id, i);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    pthread_create(&t1, NULL, task, &id1);
    pthread_create(&t2, NULL, task, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
```

### Requirements

- GCC compiler
- POSIX thread library (`pthread`)
- Linux/Unix environment

### How to Run

```bash
gcc lab1.c -o lab1 -pthread
./lab1
```

### Expected Outcome

Students will observe that the output of the two threads is interleaved unpredictably. Sometimes Thread 1 prints first; other times Thread 2 does. This reflects the fundamental nature of concurrent execution: there is no guaranteed ordering unless explicitly enforced.

This behavior illustrates the concept of independent parallel execution and resource sharing. Although no incorrect results occur yet, this unpredictability is the root cause of more serious problems like race conditions and deadlock. The system scheduler determines execution order dynamically, which aligns with Chapter 6's emphasis that concurrency behavior depends on execution dynamics, not just program logic.

## Lab 2 - Simulating Deadlock with Threads

### Objective, Motivation, and Main Concept

This lab introduces the concept of deadlock, defined as a situation where a set of processes are permanently blocked, each waiting for a resource held by another. The objective is to demonstrate how improper resource acquisition ordering leads to deadlock.

In real-world systems, deadlock can halt entire applications, such as database systems or operating systems. For example, two services waiting indefinitely for each other's locks can cause system-wide failure.

Chapter 6 explains that deadlock occurs when four conditions hold: mutual exclusion, hold-and-wait, no preemption, and circular wait. This lab focuses on circular wait by simulating two threads acquiring resources in opposite order.

### Implementation

```c
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
```

### Expected Outcome

The program will freeze indefinitely, demonstrating deadlock. Thread 1 holds resource A and waits for B, while Thread 2 holds B and waits for A.

Students will clearly see the circular wait condition, one of the necessary conditions for deadlock. This aligns exactly with the example in Chapter 6 where two processes each hold one resource and wait for another.

## Lab 3 - Deadlock Prevention via Resource Ordering

### Objective, Motivation, and Main Concept

This lab demonstrates deadlock prevention, which works by eliminating one of the necessary conditions for deadlock. Specifically, we remove the circular wait condition by enforcing a strict order of resource acquisition.

In real systems, ordering strategies are commonly used in databases and operating systems to ensure safe resource allocation.

### Implementation

```c
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
```

### Expected Outcome

The program runs successfully without deadlock. Both threads follow the same order, preventing circular dependency.

Students observe that system design constraints can prevent deadlock, as emphasized in Chapter 6.

## Lab 4 - Dining Philosophers with Semaphores

### Objective, Motivation, and Main Concept

This lab explores the classic Dining Philosophers Problem, illustrating synchronization and deadlock.

It models real-world resource sharing, such as database locks and network access. Each philosopher (thread) competes for shared resources (forks).

### Implementation (Semaphore Solution)

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t forks[N];

void* philosopher(void* num) {
    int i = *(int*)num;

    while (1) {
        printf("Philosopher %d thinking\n", i);
        sleep(1);

        sem_wait(&forks[i]);
        sem_wait(&forks[(i + 1) % N]);

        printf("Philosopher %d eating\n", i);
        sleep(1);

        sem_post(&forks[i]);
        sem_post(&forks[(i + 1) % N]);
    }
}

int main() {
    pthread_t p[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&p[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(p[i], NULL);
    }
}
```

### Expected Outcome

Students may observe deadlock or starvation, depending on scheduling.

This directly reflects Chapter 6's discussion of the Dining Philosophers problem and semaphore-based solutions.

## Lab 5 - Blocking Synchronization using Semaphores

### Objective, Motivation, and Main Concept

This lab introduces blocking synchronization, where threads wait efficiently instead of busy-waiting.

Unlike spinning, blocking allows the OS to suspend threads, improving performance.

### Implementation (Producer-Consumer)

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5

int buffer[SIZE];
int count = 0;

sem_t empty, full, mutex;

void* producer(void* arg) {
    while (1) {
        sleep(1);  // simulate production time

        sem_wait(&empty);  // wait if buffer is full
        sem_wait(&mutex);  // enter critical section

        buffer[count++] = 1;
        printf("Produced, count = %d\n", count);

        sem_post(&mutex);  // leave critical section
        sem_post(&full);   // signal item available
    }
}

void* consumer(void* arg) {
    while (1) {
        sleep(2);  // simulate consumption time

        sem_wait(&full);   // wait if buffer is empty
        sem_wait(&mutex);  // enter critical section

        buffer[--count] = 0;
        printf("Consumed, count = %d\n", count);

        sem_post(&mutex);  // leave critical section
        sem_post(&empty);  // signal space available
    }
}
```

```c
int main() {
    pthread_t prod, cons;

    // Initialize semaphores
    sem_init(&empty, 0, SIZE);  // buffer initially empty
    sem_init(&full, 0, 0);      // no items initially
    sem_init(&mutex, 0, 1);     // binary semaphore (mutex)

    // Create threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads (they run infinitely)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Cleanup (not reached in infinite loop, but good practice)
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
```

### Expected Outcome

Threads will block and wake correctly, avoiding CPU waste.

This demonstrates efficient synchronization using semaphores, as discussed in Chapter 6 concurrency mechanisms.
