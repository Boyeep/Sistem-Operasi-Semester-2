# Hands-On 5

## Lab 1 - Creating Multiple Threads and Observing Concurrent Execution

### Objective

This lab introduces the basic idea of a thread as an independent path of execution inside a process. The goal is to help students see that a single program can contain multiple flows of control that appear to progress together. This connects directly to the Chapter 4 distinction between a process as a resource-owning unit and a thread as the unit of scheduling and execution.

### Motivation

Modern applications often perform several activities at once. A file server may handle many client requests, a spreadsheet may update calculations while still responding to user input, and a word processor may save data in the background. Threads make this possible without creating a completely separate process for each activity. Since threads inside the same process share memory and resources, they are lighter and more efficient than separate processes.

### Main Concept

A process can contain one or more threads. Each thread has its own execution state, program counter, registers, and stack, but shares the process address space and resources with other threads. In this lab, each thread prints messages independently. The exact order of output may vary because the operating system schedules threads dynamically. This demonstrates that concurrent execution does not guarantee a fixed order.

### Implementation - `lab1_basic_threads.c`

```c
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
```

### Requirements

A Linux environment with GCC and POSIX thread support.

### How to Run

```bash
gcc lab1_basic_threads.c -o lab1_basic_threads -pthread
./lab1_basic_threads
```

### Expected Outcome

Students should observe that messages from different threads are interleaved. Thread 1 may print first in one run, while Thread 2 or Thread 3 may print first in another run. This happens because thread scheduling is controlled by the operating system, not by the source-code order alone.

The important observation is that all three threads belong to the same process, but each has its own execution path. This illustrates the Chapter 4 concept of multithreading: several execution paths can exist inside one process, allowing a program to perform multiple activities concurrently.

## Lab 2 - Race Condition on Shared Data

### Objective

This lab demonstrates a race condition. Students will observe what happens when multiple threads access and modify the same shared variable without synchronization. The goal is to understand why shared memory between threads is powerful but dangerous.

### Motivation

Threads inside the same process can communicate easily because they share memory. However, this advantage can become a problem when two or more threads update the same data at the same time. Real-world systems such as banking software, ticket booking systems, counters, logs, and shared queues must prevent inconsistent updates. Without synchronization, the final result may depend on timing rather than program logic.

### Main Concept

Chapter 5 defines a race condition as a situation where multiple processes or threads read and write shared data, and the final result depends on the relative timing of execution. In this lab, two threads repeatedly increment the same counter. The operation `counter++` looks simple, but internally it involves reading the value, adding one, and writing it back. If two threads perform these steps at overlapping times, one update may overwrite the other. The result is incorrect because the critical section is not protected.

### Implementation - `lab2_race_condition.c`

```c
#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 1000000

int counter = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        counter++;
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment_counter, NULL);
    pthread_create(&t2, NULL, increment_counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected counter value: %d\n", ITERATIONS * 2);
    printf("Actual counter value  : %d\n", counter);
    return 0;
}
```

### Requirements

A Linux environment with GCC and POSIX thread support.

### How to Run

```bash
gcc lab2_race_condition.c -o lab2_race_condition -pthread
./lab2_race_condition
./lab2_race_condition
./lab2_race_condition
```

### Expected Outcome

The expected value is `2000000`, but students may observe a smaller value. The exact result can differ between runs. This variation is the key learning point.

The incorrect result happens because both threads access the shared variable without mutual exclusion. One thread may read the counter before another thread writes its updated value. As a result, one increment is lost. This directly demonstrates the Chapter 5 problem of race conditions and shows why shared data must be protected when used concurrently.

## Lab 3 - Solving the Race Condition with Mutual Exclusion

### Objective

This lab introduces mutual exclusion as a solution to the race condition observed in Lab 2. Students will protect a critical section so only one thread can update shared data at a time.

### Motivation

Many systems depend on correct updates to shared data. If a bank balance, inventory count, or shared file index is updated incorrectly, the system becomes unreliable. Mutual exclusion is one of the fundamental requirements in operating system design because it ensures that critical sections are executed safely.

### Main Concept

A critical section is a part of code that accesses shared resources and must not be executed by more than one thread at the same time. Mutual exclusion ensures that while one thread is inside the critical section, other threads must wait. In this lab, a mutex lock is used to protect `counter++`. The lock does not remove concurrency from the whole program; it only controls access to the shared update. This illustrates the Chapter 5 principle that synchronization should protect the shared resource while allowing the rest of the program to continue concurrently.

### Implementation - `lab3_mutex_counter.c`

```c
#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 1000000

int counter = 0;
pthread_mutex_t lock;

void* increment_counter(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, increment_counter, NULL);
    pthread_create(&t2, NULL, increment_counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    printf("Expected counter value: %d\n", ITERATIONS * 2);
    printf("Actual counter value  : %d\n", counter);
    return 0;
}
```

### Requirements

A Linux environment with GCC and POSIX thread support.

### How to Run

```bash
gcc lab3_mutex_counter.c -o lab3_mutex_counter -pthread
./lab3_mutex_counter
```

### Expected Outcome

The actual counter value should now match the expected value: `2000000`.

Students should observe that the result becomes stable and correct. The reason is that the mutex forces the increment operation to behave like an indivisible critical section. Only one thread can read, modify, and write the counter at a time. This connects to the Chapter 5 requirement of mutual exclusion: shared resources must be protected from simultaneous conflicting access.

## Lab 4 - Producer-Consumer Using Semaphore Synchronization

### Objective

This lab demonstrates synchronization between threads using the producer-consumer pattern. Students will learn that synchronization is not only about protecting shared data, but also about coordinating the order of execution between threads.

### Motivation

Many real systems use producer-consumer behavior. A keyboard produces input consumed by an application, a network card produces packets consumed by the operating system, and a file reader may produce data blocks consumed by a processing thread. The producer and consumer must coordinate carefully. The consumer should not consume from an empty buffer, and the producer should not overwrite a full buffer.

### Main Concept

Chapter 5 introduces the producer-consumer problem as a classic synchronization problem. This lab uses a shared buffer of size one. A producer places data into the buffer, and a consumer removes it. Two semaphores coordinate the process: one represents whether the buffer is empty, and the other represents whether it is full. A mutex protects the actual buffer access. This shows two different synchronization roles: semaphores manage ordering and blocking, while mutual exclusion protects the critical section.

### Implementation - `lab4_producer_consumer.c`

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ITEMS 10

int buffer;
sem_t empty;
sem_t full;
pthread_mutex_t lock;

void* producer(void* arg) {
    for (int item = 1; item <= ITEMS; item++) {
        sem_wait(&empty);

        pthread_mutex_lock(&lock);
        buffer = item;
        printf("Producer produced item %d\n", item);
        pthread_mutex_unlock(&lock);

        sem_post(&full);
        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&full);

        pthread_mutex_lock(&lock);
        int item = buffer;
        printf("Consumer consumed item %d\n", item);
        pthread_mutex_unlock(&lock);

        sem_post(&empty);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&lock);
    return 0;
}
```

### Requirements

A Linux environment with GCC, POSIX threads, and semaphore support.

### How to Run

```bash
gcc lab4_producer_consumer.c -o lab4_producer_consumer -pthread
./lab4_producer_consumer
```

### Expected Outcome

Students should observe that each produced item is consumed exactly once. The consumer does not consume before the producer produces, and the producer does not overwrite the buffer before the consumer consumes the previous item.

This happens because `sem_wait(&full)` blocks the consumer when no item is available, while `sem_wait(&empty)` blocks the producer when the buffer is not empty. The mutex then protects the shared buffer during access. This lab shows that synchronization can control both safety and ordering.

## Lab 5 - Blocking and Unblocking Threads with Condition Variables

### Objective

This lab demonstrates blocking behavior in thread synchronization. Students will observe how a thread can voluntarily wait until a required condition becomes true, and how another thread can signal it to continue.

### Motivation

In many concurrent systems, a thread cannot always continue immediately. A worker may need to wait for data, a printer thread may wait for a print job, or a server thread may wait for a request. Busy waiting wastes processor time because the thread repeatedly checks a condition without doing useful work. Blocking is more efficient because the waiting thread stops running until it is notified.

### Main Concept

Chapter 4 explains that threads can move between running, ready, and blocked states. Chapter 5 explains that synchronization mechanisms are needed when threads interact through shared resources. In this lab, one thread waits until a shared condition becomes true. Another thread changes the condition and signals the waiting thread. This demonstrates blocking synchronization: a thread does not spin repeatedly; it waits until the event it needs occurs. The mutex protects the shared condition, while the condition variable provides waiting and notification behavior.

### Implementation - `lab5_blocking_condition.c`

```c
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
```

```c
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
```

### Requirements

A Linux environment with GCC and POSIX thread support.

### How to Run

```bash
gcc lab5_blocking_condition.c -o lab5_blocking_condition -pthread
./lab5_blocking_condition
```

### Expected Outcome

Students should first see the worker announce that the data is not ready and that it is blocking. After about three seconds, the controller prepares the data and signals the worker. The worker then wakes up and prints the received value.

The important observation is that the worker does not repeatedly check the variable while wasting processor time. Instead, it enters a blocked state until the condition is signaled. This connects Chapter 4 thread states with Chapter 5 synchronization: correct concurrent programs must not only protect shared resources, but also coordinate when threads are allowed to proceed.

## Lab 6 - Implementing a Critical Section with Threads

### Title

Protecting a Shared Bank Account Balance Using a Critical Section

### Objective

This exercise teaches how to identify and implement a critical section in a threaded program. Students learn that a critical section is not the whole program, but only the part of code where shared data is accessed and modified.

### Motivation

In real systems, many users may access the same resource at the same time. For example, two ATM transactions may update the same bank account balance. Without protection, both transactions may read the same old balance and overwrite each other's updates. This can produce incorrect financial data. A critical section prevents this by allowing only one thread to execute the sensitive update at a time.

### Main Concept

A critical section is a section of code that accesses a shared resource and must not be executed concurrently by multiple threads. In this case, the shared resource is `balance`. The deposit operation reads the current balance, calculates a new value, and writes it back. Although this looks like one operation, it consists of multiple steps. A mutex is used to enforce mutual exclusion around this critical section.

### Implementation - `critical_section_bank.c`

```c
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
```

```c
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
```

### Requirements

Linux environment, GCC compiler, and POSIX thread library.

### How to Run

```bash
gcc critical_section_bank.c -o critical_section_bank -pthread
./critical_section_bank
```

### Expected Outcome

Students should observe that only one thread at a time reads and updates the balance. Even though multiple threads exist, the balance update happens in a controlled sequence.

The final balance should be:

```text
1500
```

This happens because the mutex protects the critical section. While one thread is updating `balance`, other threads must wait. This demonstrates mutual exclusion: shared data is protected so concurrent execution does not produce inconsistent results.

## Lab 7 - Observing a Race Condition

### Objective

The purpose of this lab is to demonstrate how concurrent threads accessing shared data without synchronization can produce incorrect results. Students will learn to identify a race condition and understand that even simple operations can become unsafe in concurrent environments.

### Motivation

In real systems such as financial transactions or shared counters, incorrect updates can lead to serious failures. The danger is subtle because the code may appear correct but produces inconsistent results due to unpredictable execution timing.

### Main Concept

A race condition occurs when multiple threads access shared data and the result depends on execution timing. The increment operation is not atomic - it consists of read, modify, and write steps. Without protection, two threads can overlap these steps and lose updates. This demonstrates why mutual exclusion is required.

### Implementation - `lab1_race.c`

```c
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
```

### How to Run

```bash
gcc lab1_race.c -o lab1_race -pthread
./lab1_race
```

### Expected Outcome

The result will often be less than expected. This shows that concurrent updates interfere with each other. The incorrect value proves that the program contains a race condition.

## Lab 8 - Implementing Mutual Exclusion with Mutex

### Objective

To eliminate the race condition by enforcing mutual exclusion using a lock.

### Motivation

Real systems must guarantee correctness when accessing shared data. Mutual exclusion ensures that only one thread executes the critical section at a time.

### Main Concept

A critical section is the part of code that accesses shared resources. Mutual exclusion ensures that only one thread can execute it at any moment. A mutex acts as a gate: threads must acquire the lock before entering and release it after leaving.

### Implementation - `lab2_mutex.c`

```c
#include <stdio.h>
#include <pthread.h>

#define ITER 1000000

int counter = 0;
pthread_mutex_t lock;

void* increment(void* arg) {
    for (int i = 0; i < ITER; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    printf("Expected: %d\n", ITER * 2);
    printf("Actual  : %d\n", counter);

    return 0;
}
```

### Expected Outcome

The result is always correct. Students observe that synchronization ensures predictable behavior.

## Lab 9 - Semaphore-Based Mutual Exclusion

### Objective

To implement mutual exclusion using a semaphore instead of a mutex.

### Motivation

Semaphores are a more general synchronization tool used in operating systems to manage resource access and coordination between processes.

### Main Concept

A binary semaphore acts like a lock. The `wait` operation decrements the semaphore and blocks if it is zero. The `signal` operation increments it, allowing other threads to proceed. This enforces mutual exclusion over a critical section.

### Implementation - `lab3_semaphore.c`

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define ITER 1000000

int counter = 0;
sem_t sem;

void* increment(void* arg) {
    for (int i = 0; i < ITER; i++) {
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    sem_init(&sem, 0, 1);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem);

    printf("Expected: %d\n", ITER * 2);
    printf("Actual  : %d\n", counter);

    return 0;
}
```

### Expected Outcome

Correct final value. Students learn that semaphores can enforce mutual exclusion similarly to mutexes but with broader applications.

## Lab 10 - Producer-Consumer Synchronization

### Objective

To coordinate two threads so that one produces data and the other consumes it safely.

### Motivation

Many systems rely on data pipelines where one component produces and another consumes. Without coordination, data may be lost or overwritten.

### Main Concept

The producer-consumer problem requires synchronization to ensure:

- Producer does not overwrite unconsumed data
- Consumer does not read empty data

Semaphores manage availability, while mutual exclusion protects the shared buffer.

### Implementation - `lab4_producer_consumer.c`

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int buffer;
sem_t empty, full;
pthread_mutex_t lock;

void* producer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        sem_wait(&empty);

        pthread_mutex_lock(&lock);
        buffer = i;
        printf("Produced: %d\n", i);
        pthread_mutex_unlock(&lock);

        sem_post(&full);
        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        sem_wait(&full);

        pthread_mutex_lock(&lock);
        printf("Consumed: %d\n", buffer);
        pthread_mutex_unlock(&lock);

        sem_post(&empty);
        sleep(1);
    }

    return NULL;
}
```

```c
int main() {
    pthread_t p, c;

    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}
```

### Expected Outcome

Students observe strict alternation between production and consumption, demonstrating synchronization beyond simple mutual exclusion.

## Lab 11 - Readers-Writers Problem

### Objective

To explore a scenario where multiple readers can access data simultaneously, but writers require exclusive access.

### Motivation

Database systems, file systems, and shared memory structures often require this pattern. Allowing multiple readers improves performance, but writers must be protected.

### Main Concept

The readers-writers problem balances concurrency and safety. Multiple readers can enter simultaneously, but a writer must wait until all readers exit. This requires careful synchronization to avoid conflicts.

### Implementation - `lab5_readers_writers.c`

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int read_count = 0;
pthread_mutex_t mutex;
pthread_mutex_t write_lock;

void* reader(void* arg) {
    pthread_mutex_lock(&mutex);
    read_count++;

    if (read_count == 1) {
        pthread_mutex_lock(&write_lock);
    }

    pthread_mutex_unlock(&mutex);

    printf("Reader reading\n");
    sleep(1);

    pthread_mutex_lock(&mutex);
    read_count--;

    if (read_count == 0) {
        pthread_mutex_unlock(&write_lock);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}
```

```c
void* writer(void* arg) {
    pthread_mutex_lock(&write_lock);

    printf("Writer writing\n");
    sleep(2);

    pthread_mutex_unlock(&write_lock);

    return NULL;
}

int main() {
    pthread_t r1, r2, w1;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_lock, NULL);

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w1, NULL);

    return 0;
}
```

### Expected Outcome

Multiple readers execute concurrently, but the writer runs alone. Students observe controlled concurrency and synchronization policies.
