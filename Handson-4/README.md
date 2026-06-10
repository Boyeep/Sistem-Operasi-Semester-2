# Hands-On 4: Threads and Concurrency (Bash & C)

## Lab 1 (Bash): Sequential vs Concurrent Execution

### Objective, Motivation, and Main Concept

The objective of this lab is to help students compare sequential execution and concurrent execution in a simple, visible way. Sequential execution means that one task must finish before the next task begins. This is easy to understand and predictable, but it can waste time when tasks are independent or spend much of their time waiting. Concurrent execution allows several tasks to make progress during overlapping time periods.

The motivation is that modern operating systems rarely execute only one activity at a time. A computer may download files, respond to user input, play audio, and update the display at the same time. Threads and processes make this possible by allowing multiple execution flows to exist. Even if the machine has only one processor core, the operating system can switch between tasks quickly enough to create the appearance of simultaneous progress. On multicore systems, some tasks may actually run in parallel.

The main concept in this lab is concurrency through background execution. In Bash, the `&` symbol tells the shell to start a command in the background. The `wait` command tells the script to pause until all background jobs finish. Students should understand that concurrency does not always mean all the tasks are part of the same process. In this Bash example, the concurrent tasks are separate background processes, but the idea prepares students for thread-based concurrency later.

### Implementation (Script)

```bash
#!/bin/bash

task() {
    echo "Task $1 started"
    sleep 3
    echo "Task $1 finished"
}

echo "Sequential execution"
task A
task B
task C

echo "Concurrent execution"
task A &
task B &
task C &

wait
echo "All tasks completed"
```

### Requirements to Run

- Linux or macOS terminal, or Windows with WSL/Git Bash
- Bash shell
- Basic terminal command knowledge

### How to Run / Execute

```bash
chmod +x lab1.sh
./lab1.sh
```

### Expected Outcome

Students should observe that the sequential part takes about 9 seconds because Task A sleeps for 3 seconds, then Task B sleeps for 3 seconds, and finally Task C sleeps for 3 seconds. The total time is approximately the sum of all waiting times. This shows the cost of sequential execution when tasks are independent.

In the concurrent part, all three tasks are started almost immediately. They each sleep for about 3 seconds at the same time, so the total time is about 3 seconds instead of 9 seconds. The output order may not always be exactly the same on every run because the operating system decides how to schedule the background processes.

The important observation is not only that concurrent execution is faster, but why it is faster: the tasks do not depend on each other, so they can overlap. This illustrates one of the major benefits of threads and concurrent execution in operating systems: better use of waiting time. However, this lab also introduces an important limitation. Since the tasks are independent and do not share data, concurrency is safe here. Later labs show that when tasks share data, concurrency can cause errors if not controlled.

## Lab 2 (Bash): Foreground and Background Tasks

### Objective, Motivation, and Main Concept

The objective of this lab is to demonstrate how a system can continue interacting with a user while another task runs in the background. Students will observe that one execution flow can handle user input while another execution flow performs periodic work. This is a practical model of how real applications maintain responsiveness.

The motivation comes from everyday software. A text editor may autosave while the user types. A messaging application may receive messages while the user writes a reply. A web browser may load resources while still allowing scrolling and clicking. These applications would feel slow or frozen if every background operation blocked the user interface. Threads solve this by separating responsibilities into different execution flows.

The main concept is foreground and background execution. The foreground task is the part of the program that interacts directly with the user. The background task runs without requiring immediate user input. In Bash, this is simulated by running one loop in the background using `&`, while another loop remains in the foreground and accepts input.

### Implementation (Script)

```bash
#!/bin/bash

background_task() {
    while true; do
        echo "[Background] Periodic task is running..."
        sleep 5
    done
}

foreground_task() {
    while true; do
        read -p "Enter command: " cmd
        echo "You typed: $cmd"
    done
}

background_task &
foreground_task
```

### Requirements to Run

- Bash shell
- Terminal that supports interactive input

### How to Run / Execute

```bash
chmod +x lab2.sh
./lab2.sh
```

Stop the program with `Ctrl + C`.

### Expected Outcome

Students should observe that the script continues to accept user input even while the background task prints a message every 5 seconds. The user can type commands, press Enter, and receive a response without waiting for the background task to finish. This demonstrates responsiveness.

The background message may sometimes appear while the user is typing. This is normal and important to observe. It shows that two execution flows are active at the same time from the user's perspective. The terminal output may look slightly messy because both the foreground and background processes write to the same screen. This is also an important concurrency observation: shared output devices can become confusing when multiple tasks write to them simultaneously.

The deeper lesson is that concurrency improves user experience, but it also introduces coordination challenges. The program becomes more responsive because the foreground task is not forced to wait for the background task. However, both tasks share the terminal, so their outputs may interfere visually. This prepares students for later discussions about shared resources and synchronization.

## Lab 3 (Bash): Race Condition on a Shared File

### Objective, Motivation, and Main Concept

The objective of this lab is to demonstrate a race condition. A race condition occurs when multiple execution flows access and modify shared data at the same time, and the final result depends on timing. Students will see that concurrent programs can produce incorrect results even when each individual instruction appears logically correct.

The motivation is that many real systems share resources. Bank accounts, inventory systems, database records, log files, counters, and configuration files may all be accessed by multiple processes or threads. If these accesses are not controlled, data can be overwritten or lost. This lab makes the problem visible using a simple counter stored in a file.

The main concept is unsynchronized access to a shared resource. Each process reads the current value, adds one, and writes the new value back. The problem is that these three steps are not atomic. Another process may read the same old value before the first process writes the updated value. As a result, two increments may become only one effective increment.

### Implementation (Script)

```bash
#!/bin/bash

echo 0 > counter.txt

increment() {
    for i in {1..100}; do
        count=$(cat counter.txt)
        count=$((count + 1))
        echo $count > counter.txt
    done
}

increment &
increment &
increment &

wait
echo "Final counter value:"
cat counter.txt
```

### Requirements to Run

- Bash shell
- Permission to create files in the current directory

### How to Run / Execute

```bash
chmod +x lab3.sh
./lab3.sh
```

Run it several times.

### Expected Outcome

The expected mathematical result is 300 because three background processes each increment the counter 100 times. However, students will often observe a value less than 300. The exact value may change from one run to another. This inconsistent result is the key learning outcome.

The incorrect result happens because the file update operation is not protected. Suppose two processes read the value `10` at nearly the same time. Both calculate `11`, and both write `11` back to the file. Even though two increments occurred logically, the stored result increased only once. This is called a lost update.

Students should understand that the error is not caused by a syntax mistake. The logic seems correct when viewed from a single-process perspective. The error appears only because several processes execute the same logic concurrently. This is why concurrent programming is difficult: correctness depends not only on what instructions are written, but also on when they are executed relative to other tasks.

This lab should lead students to conclude that shared resources require coordination. Without synchronization, concurrency may improve speed but damage correctness. This prepares students for the next lab, where a lock is introduced to protect the critical section.

## Lab 4 (Bash): Fixing Race Condition with a Lock

### Objective, Motivation, and Main Concept

The objective of this lab is to fix the race condition from the previous lab using a simple lock. Students will learn that shared resources must be protected when multiple execution flows access them. The goal is not only to produce the correct result, but also to understand why locking works.

The motivation is that operating systems and applications must preserve correctness even under concurrency. If two threads update the same data structure at the same time, the structure may become corrupted. If two processes write to the same file without coordination, data may be lost. Synchronization mechanisms exist to control access to shared resources.

The main concept is mutual exclusion. Mutual exclusion means only one process or thread is allowed inside a critical section at a time. A critical section is the part of the program that accesses shared data. In this lab, reading the counter, incrementing it, and writing it back form the critical section. The lock directory acts as a simple mutex: if a process successfully creates the directory, it owns the lock; if not, it waits.

### Implementation (Script)

```bash
#!/bin/bash

echo 0 > counter.txt
lockdir="lockdir"

increment() {
    for i in {1..100}; do
        while ! mkdir "$lockdir" 2>/dev/null; do
            sleep 0.01
        done

        count=$(cat counter.txt)
        count=$((count + 1))
        echo $count > counter.txt

        rmdir "$lockdir"
    done
}

increment &
increment &
increment &

wait
echo "Final counter value:"
cat counter.txt
```

### Requirements to Run

- Bash shell
- Permission to create directories and files

### How to Run / Execute

```bash
chmod +x lab4.sh
./lab4.sh
```

### Expected Outcome

Students should observe that the final counter value is now consistently 300. This shows that the lock prevents multiple processes from updating the counter at the same time. Unlike the previous lab, the output should no longer vary across runs under normal conditions.

The reason the result becomes correct is that the critical section is now protected. Only the process that successfully creates `lockdir` may read, increment, and write the counter. Other processes must wait until the lock directory is removed. This prevents the lost update problem because no two processes can read the same counter value as part of overlapping updates.

Students should also observe that correctness may come with a performance cost. Because only one process can enter the critical section at a time, part of the program becomes serialized. This is a key trade-off in concurrent system design: synchronization protects data, but too much synchronization can reduce parallelism.

The deeper expected understanding is that synchronization is not optional when shared mutable data exists. The lock does not make the program more concurrent; instead, it makes the concurrent program correct. This distinction is central to operating systems, thread programming, and database systems.

## Lab 5 (Bash): Blocking Simulation

### Objective, Motivation, and Main Concept

The objective of this lab is to show that one blocked task does not need to stop other tasks. Students will simulate a slow operation using `sleep` while another task continues to run. This helps explain why operating systems use concurrency to improve responsiveness.

The motivation comes from I/O operations. Programs often wait for input, disk access, network responses, or timers. If the whole program or system stopped whenever one activity waited, computers would be inefficient and frustrating to use. Threads and processes allow useful work to continue while one task is blocked.

The main concept is blocking and independent progress. A blocking operation pauses one execution flow, but other execution flows may continue. In operating systems, when a thread waits for I/O, the scheduler can run another ready thread. This improves CPU utilization and system responsiveness.

### Implementation (Script)

```bash
#!/bin/bash

(sleep 5; echo "Task A completed after waiting") &

(for i in {1..5}; do
    echo "Task B is working: step $i"
    sleep 1
done) &

wait
echo "Both tasks finished"
```

### Requirements to Run

- Bash shell

### How to Run / Execute

```bash
chmod +x lab5.sh
./lab5.sh
```

### Expected Outcome

Students should observe that Task A waits silently for about 5 seconds, while Task B continues printing progress messages once per second. Task B does not wait for Task A to finish. Both tasks complete at roughly the same time.

This outcome demonstrates that blocking is local to the task that performs the blocking operation. In this example, `sleep 5` blocks Task A, but Task B remains runnable. The shell and operating system allow Task B to continue executing.

The important conceptual outcome is that concurrency helps systems remain productive during waiting periods. If the tasks were sequential, Task B would not start until Task A finished waiting. In the concurrent version, the waiting time of Task A overlaps with the work of Task B.

Students should connect this behavior to threads in real operating systems. When one thread waits for I/O, another thread in the same process or another process can continue. This is one reason multithreaded applications can remain responsive even when some operations are slow.

## Lab 6 (C): Sequential vs Multithread Execution

### Objective, Motivation, and Main Concept

The objective of this lab is to compare normal function execution with thread-based execution in C. Students will see that functions called sequentially run one after another, while functions launched as threads can overlap in time.

The motivation is that threads are a major operating system abstraction. A process owns resources, but threads represent execution paths inside that process. Multithreading allows a program to divide work into multiple flows, improving responsiveness and sometimes performance.

The main concept is thread creation and joining. `pthread_create` starts a new thread, and `pthread_join` waits for a thread to finish. This allows the main program to coordinate thread completion.

### Implementation (Program)

```c
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
```

### Requirements to Run

- GCC compiler
- POSIX thread library
- Linux/macOS/WSL environment

### How to Run / Execute

```bash
gcc lab6.c -o lab6 -pthread
./lab6
```

### Expected Outcome

Students should observe that the three tasks start close together and finish after approximately 3 seconds. The start messages for Task A, Task B, and Task C should appear before all tasks finish, although the exact order may vary.

This outcome demonstrates that each task is running in its own thread. Instead of waiting for Task A to finish before starting Task B, the program creates all three threads and then waits for them using `pthread_join`. The join calls are important because they prevent the main program from ending before the worker threads finish.

Students should also notice that output order is not guaranteed. One run may print Task A first, while another run may print Task B or C first. This happens because thread scheduling is controlled by the operating system. The programmer creates the threads, but the OS decides when each one runs.

The deeper lesson is that multithreading introduces nondeterminism in execution order. This is acceptable when tasks are independent, but it becomes dangerous when tasks share data without protection. Later labs explore that problem.

## Lab 7 (C): Background Thread inside One Process

### Objective, Motivation, and Main Concept

The objective of this lab is to demonstrate how one C program can contain multiple active threads with different responsibilities. One thread performs periodic background work while the main thread continues its own activity.

The motivation is based on real application design. Many programs separate work into background and foreground responsibilities. For example, a program may refresh data, save files, or monitor events in one thread while another thread handles user-facing work. This structure improves modularity and responsiveness.

The main concept is that threads share a process but have separate execution paths. The background thread and main thread exist in the same process, but each has its own flow of control. This is different from Bash background processes, which are separate processes.

### Implementation (Program)

```c
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
```

### Requirements to Run

- GCC compiler
- pthread support

### How to Run / Execute

```bash
gcc lab7.c -o lab7 -pthread
./lab7
```

### Expected Outcome

Students should observe that the main thread prints a message every second, while the background thread prints a message every 3 seconds. Both outputs appear during the same program execution. The background thread does not prevent the main thread from continuing.

This outcome demonstrates that multiple threads can exist inside one process and perform different roles. The main thread represents the primary flow of the program, while the background thread represents supporting work. This is a common design pattern in responsive applications.

Students should also notice that when the main function finishes, the process terminates, and the background thread stops as well. This is an important thread lifecycle observation. Threads belong to a process; when the process ends, its threads end too.

The deeper conceptual outcome is that threads allow a program to be structured around independent responsibilities. However, if these threads later share data, synchronization becomes necessary. This lab focuses on independent behavior before introducing shared-state problems.

## Lab 8 (C): Race Condition with Shared Variable

### Objective, Motivation, and Main Concept

The objective of this lab is to show a race condition in true multithreading. Multiple threads will increment the same global variable without synchronization. Students will observe that the final result may be incorrect even though the code appears simple.

The motivation is that threads inside the same process share memory. This makes communication easy and efficient, but it also makes errors easier to create. A shared variable can be accessed by all threads, so uncontrolled updates may interfere with each other.

The main concept is that `counter++` is not atomic. It usually involves reading the value, adding one, and writing the value back. If two threads perform these steps at overlapping times, updates can be lost.

### Implementation (Program)

```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;
    }

    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_create(&t3, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Expected counter: 300000\n");
    printf("Actual counter  : %d\n", counter);
    return 0;
}
```

### Requirements to Run

- GCC compiler
- pthread support

### How to Run / Execute

```bash
gcc lab8.c -o lab8 -pthread
./lab8
```

Run it several times.

### Expected Outcome

Students should expect the mathematically correct result to be 300000 because three threads each increment the counter 100000 times. However, the actual result may be less than 300000, and it may differ each time the program runs.

This happens because the increment operation is not atomic. When one thread executes `counter++`, it may read the current value into a register, add one, and then write the result back. If another thread reads the same old value before the first thread writes its result, one update can overwrite another. This creates lost updates.

The nondeterministic result is the most important observation. The program may sometimes appear closer to correct and sometimes much lower, depending on scheduling. This demonstrates that race conditions can be difficult to detect because they depend on timing.

The deeper lesson is that shared memory is powerful but dangerous. Threads communicate easily through shared variables, but correctness requires synchronization. Students should understand that race conditions are not solved by hoping the scheduler behaves nicely; they must be prevented by design.

## Lab 9 (C): Fix Race Condition with Mutex

### Objective, Motivation, and Main Concept

The objective of this lab is to correct the race condition using a mutex. Students will protect the shared counter so only one thread can update it at a time.

The motivation is that real multithreaded programs frequently share data structures such as queues, counters, buffers, and files. Without protection, these structures can become inconsistent. A mutex provides a standard synchronization tool for enforcing mutual exclusion.

The main concept is the critical section. The critical section is the part of the code that accesses shared mutable data. `pthread_mutex_lock` marks the beginning of exclusive access, and `pthread_mutex_unlock` releases it.

### Implementation (Program)

```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock;

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_create(&t3, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Expected counter: 300000\n");
    printf("Actual counter  : %d\n", counter);

    pthread_mutex_destroy(&lock);
    return 0;
}
```

### Requirements to Run

- GCC compiler
- pthread support

### How to Run / Execute

```bash
gcc lab9.c -o lab9 -pthread
./lab9
```

### Expected Outcome

Students should observe that the actual counter value is consistently 300000. Unlike the previous lab, repeated runs should produce the same correct result. This shows that the mutex successfully protects the shared variable.

The reason the result becomes correct is that only one thread can execute the critical section at a time. When a thread locks the mutex, other threads attempting to lock it must wait. After the first thread increments the counter and unlocks the mutex, another thread may enter. This prevents overlapping read-modify-write sequences.

Students should also understand the performance implication. The counter update is now safe, but it is also serialized. Even though there are three threads, only one can update the counter at any exact moment. This is a necessary trade-off when protecting shared data.

The deeper expected outcome is that synchronization restores correctness, but careful design is needed to avoid unnecessary locking. In real systems, programmers try to make critical sections as small as possible so that threads spend less time waiting.

## Lab 10 (C): Blocking Threads

### Objective, Motivation, and Main Concept

The objective of this lab is to demonstrate that when one thread is blocked, another thread can continue executing. This reinforces the usefulness of threads for programs that perform slow or waiting operations.

The motivation comes from real applications that handle I/O, network requests, timers, or user input. If one operation blocks, the entire application should not necessarily stop. Threads allow slow operations to be isolated so other work can continue.

The main concept is blocking at the thread level. A blocking call pauses the thread that calls it, but the scheduler can continue running other ready threads in the same process.

### Implementation (Program)

```c
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
```

### Requirements to Run

- GCC compiler
- pthread support

### How to Run / Execute

```bash
gcc lab10.c -o lab10 -pthread
./lab10
```

### Expected Outcome

Students should observe that Thread A begins by waiting for simulated I/O, while Thread B continues printing work steps once per second. Thread B does not stop simply because Thread A is sleeping. After about 6 seconds, Thread A reports that its simulated I/O is complete, and the program finishes after both threads join.

This demonstrates that blocking is associated with a specific thread, not necessarily the entire process. The operating system can schedule Thread B while Thread A is blocked. This is one of the main practical advantages of multithreading.

Students should compare this with a single-threaded program. In a single-threaded version, the program would wait for the slow task before doing other work. In this multithreaded version, the waiting time is overlapped with useful activity.

The deeper conceptual outcome is that threads improve responsiveness and resource usage, especially when programs contain operations with different timing behavior. However, this benefit is safest when threads perform independent work or when shared data is properly synchronized.

