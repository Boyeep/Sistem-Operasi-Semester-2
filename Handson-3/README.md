# Handson-3: process basics and simple process coordination in Bash

## Lab 1: Creating and Observing a Simple Process

### Objective

Students understand how a process is created from Bash, how to run it in the foreground and background, and how to observe its PID and basic status.

### Motivation

In operating systems, almost every activity is executed in the form of a process. When we run a program from the shell, we are actually creating a new process. Understanding this is the foundation for larger topics such as scheduling, synchronization, monitoring, and system automation. At the beginner level, students need to directly see that one script can spawn another process, that every process has its own PID, and that a process can run either in the foreground or the background. This hands-on exercise is important because students often run commands without fully understanding what happens behind the scenes. Through this exercise, they will see how Bash can control processes in a simple way and how tools like `ps` can be used for observation.

### Main Concepts

- Running a process from Bash
- Foreground vs background
- Process PID
- Observing process status with `ps`

### Script

Create a file named `task1_process_basic.sh`:

```bash
#!/bin/bash

echo "Running a sleep process in the background..."
sleep 30 &
PID_BG=$!

echo "Background process PID: $PID_BG"
echo "Displaying process status with ps:"
ps -p $PID_BG -o pid,ppid,stat,cmd

echo "Waiting for 3 seconds..."
sleep 3

echo "Checking process status again:"
ps -p $PID_BG -o pid,ppid,stat,cmd

echo "Stopping the process..."
kill $PID_BG

echo "Status after kill:"
ps -p $PID_BG -o pid,ppid,stat,cmd
```

### Practice Steps

1. Save the script as `task1_process_basic.sh`.
2. Make it executable:

```bash
chmod +x task1_process_basic.sh
```

3. Run it:

```bash
./task1_process_basic.sh
```

### Script Explanation

- `sleep 30 &` runs the `sleep` process in the background.
- The `&` symbol means the shell does not wait for the process to finish.
- `$!` stores the PID of the last background process.
- `ps -p PID -o pid,ppid,stat,cmd` displays information about a specific process.
- `kill PID` sends a termination signal to the process.

### What to Observe

- The PID of the created process
- The process status, for example `S` for sleeping
- The change in status after a `kill` signal is sent

### Expected Outcome

Students see that Bash can create a new process and place it in the background. They also see that a process can be observed using `ps`, has its own PID, and can be terminated manually. This becomes the foundation for understanding the process lifecycle.

## Lab 2: Monitoring the Status of Another Process

### Objective

Students understand how one process or script can check the status of another process and make decisions based on that status.

### Motivation

In many simple systems, one process often needs to know whether another process is still running, has already finished, or has failed. A real example could be a monitoring script that must ensure a service is still active, or a small data pipeline that waits for a previous task to finish. For students, this is an entry point to understanding process coordination. Without the ability to check status, a script cannot build an orderly workflow. This hands-on exercise is important because it shows that Bash is not only a tool for executing commands, but also a control tool that can make decisions based on the condition of another process in real time.

### Main Concepts

- Checking whether a process is still alive
- Using `kill -0`
- Exit status
- Simple monitoring loop

### Script

Create a file `task2_check_process_status.sh`:

```bash
#!/bin/bash

echo "Starting worker process..."

sleep 10 &

WORKER_PID=$!

echo "Worker PID: $WORKER_PID"

while true
do
    if kill -0 $WORKER_PID 2>/dev/null; then
        echo "[$(date +%H:%M:%S)] Worker is still running..."
    else
        echo "[$(date +%H:%M:%S)] Worker has finished."
        break
    fi

    sleep 2
done

echo "Monitoring script finished."
```

### Practice Steps

1. Save it as `task2_check_process_status.sh`.
2. Make it executable:

```bash
chmod +x task2_check_process_status.sh
```

3. Run it:

```bash
./task2_check_process_status.sh
```

### Script Explanation

- `sleep 10 &` creates a simple worker process.
- `kill -0 PID` does not kill the process, it only checks whether the process exists and can be accessed.
- `2>/dev/null` hides the error message if the process no longer exists.
- The `while true` loop performs polling every 2 seconds.

### What to Observe

- The main script continuously monitors the worker
- When the worker finishes, the loop stops
- Process status can be used for decision-making between processes

### Expected Outcome

Students understand that the status of another process can be monitored from Bash. They learn that basic coordination can be done without complex tools, using only PID and simple checks. This is very important before moving to more advanced synchronization.

## Lab 3: Interacting Between Processes Using a Signal File

### Objective

Students understand how two processes can interact in a simple way by using a file as a communication medium.

### Motivation

At the early stage of learning about processes, inter-process communication often feels abstract. However, the basic idea can be introduced with a very simple method, such as using a status file or signal file. In real practice, this approach is not the most advanced form of IPC, but it is excellent for learning because it is easy to observe. By seeing that one process creates a file and another process waits for that file to appear, students will understand the basic concept of coordination and communication between processes. This exercise builds intuition that processes do not always run independently; they often need to exchange information so work can proceed in the correct order.

### Main Concepts

- Process interaction
- File polling
- Simple synchronization
- Filesystem-based communication

### Script

Create a file `task3_process_interaction.sh`:

```bash
#!/bin/bash

SIGNAL_FILE="/tmp/process_done.signal"

rm -f "$SIGNAL_FILE"

producer() {
    echo "Producer: starting work..."

    sleep 5

    echo "Producer: work completed." > "$SIGNAL_FILE"

    echo "Producer: signal file created."
}

consumer() {
    echo "Consumer: waiting for signal from producer..."

    while [ ! -f "$SIGNAL_FILE" ]
    do
        echo "Consumer: signal not found yet, checking again..."
        sleep 1
    done

    echo "Consumer: signal received."

    echo "Signal content:"

    cat "$SIGNAL_FILE"
}

producer &

PID_PRODUCER=$!

consumer &

PID_CONSUMER=$!

wait $PID_PRODUCER

wait $PID_CONSUMER

rm -f "$SIGNAL_FILE"

echo "All processes completed."
```

### Practice Steps

1. Save the script as `task3_process_interaction.sh`
2. Make it executable:

```bash
chmod +x task3_process_interaction.sh
```

3. Run it:

```bash
./task3_process_interaction.sh
```

### Script Explanation

- `producer` simulates a process that works for 5 seconds and then creates a signal file.
- `consumer` continuously checks for the existence of that file.
- When the file appears, the consumer reads its content.
- `wait` ensures the main shell waits for both processes to finish.

### What to Observe

- Two processes run concurrently
- One process waits for the result of another
- A file can be used as a simple communication medium

### Expected Outcome

Students understand the basic idea of inter-process interaction. They see that even without using pipes, sockets, or shared memory, simple coordination can still be built. This exercise is very suitable for connecting synchronization theory with easy-to-understand implementation.

## Lab 4: Observing the Process Lifecycle (start, running, stop, terminated)

### Objective

Students understand the life stages of a process: created, running, waiting, receiving a signal, and terminating.

### Motivation

Students often think of a process only as "a program that is running." In reality, a process has a clear lifecycle: it is born when executed, exists in a certain state while running, can be stopped, can receive signals, and eventually terminates. Understanding the lifecycle is important for debugging, service monitoring, and system administration. In the real world, many system problems appear not because a program is completely wrong, but because its lifecycle is not managed properly. Through this hands-on exercise, students will see the relationship between Bash commands, process states, and OS signals more concretely.

### Main Concepts

- Process lifecycle
- Signal handling
- `trap` in Bash
- Controlled process termination

### Script

Create a file `task4_process_lifecycle.sh`:

```bash
#!/bin/bash

cleanup() {

    echo "Process received termination signal."

    echo "Performing cleanup before exiting..."

    exit 0

}

trap cleanup SIGTERM SIGINT

echo "Process started with PID $$"

COUNT=1

while true
do
    echo "Process is running... iteration $COUNT"

    COUNT=$((COUNT + 1))

    sleep 2
done
```

Run this script in one terminal, then send a signal from another terminal:

```bash
kill -TERM <PID>
```

### Practice Steps

1. Save it as `task4_process_lifecycle.sh`
2. Make it executable:

```bash
chmod +x task4_process_lifecycle.sh
```

3. Run it in the first terminal:

```bash
./task4_process_lifecycle.sh
```

4. Note the PID displayed.

From the second terminal, check its status:

```bash
ps -p <PID> -o pid,ppid,stat,cmd
```

5. Send a terminate signal:

```bash
kill -TERM <PID>
```

### Script Explanation

- `trap cleanup SIGTERM SIGINT` makes the script catch certain signals.
- `$$` prints the PID of the currently running shell script.
- The infinite loop simulates an active process.
- When it receives `SIGTERM` or `SIGINT`, the `cleanup` function is executed before exiting.

### What to Observe

- The process keeps running in a loop
- The process can be observed from another terminal
- When a signal is sent, the process does not just "die silently," but performs cleanup first

### Expected Outcome

Students understand that a process has a lifecycle that can be observed and controlled. They also learn the important concept that properly stopping a process often requires cleanup, not simply killing it forcefully.

## Lab 5: Inter-Process Dependency in a Simple Workflow

### Objective

Students understand that one process may depend on the result of another process, and Bash can be used to manage the order of such dependencies.

### Motivation

In many real workflows, one task cannot start before the previous task has completed. A simple example is data collection, followed by validation, followed by reporting. If validation is executed before the data is available, the workflow fails. This is the concept of process dependency. For students, this concept is very important because it becomes a bridge toward system automation, job scheduling, and data pipelines. This hands-on exercise shows that even at a simple level, Bash is already powerful enough to build step-by-step workflows with clear dependency control.

### Main Concepts

- Inter-process dependency
- Waiting for a process with `wait`
- Checking the result of a previous step
- Sequential workflow

### Script

Create a file `task5_process_dependency.sh`:

```bash
#!/bin/bash

DATA_FILE="/tmp/sample_data.txt"

RESULT_FILE="/tmp/report.txt"

rm -f "$DATA_FILE" "$RESULT_FILE"

download_data() {

    echo "Step 1: Collecting data..."

    sleep 3

    echo -e "value1\nvalue2\nvalue3" > "$DATA_FILE"

    echo "Step 1 completed: data saved in $DATA_FILE"

}

validate_data() {

    echo "Step 2: Validating data..."

    if [ ! -f "$DATA_FILE" ]; then

        echo "ERROR: data is not available yet."

        return 1

    fi

    if [ ! -s "$DATA_FILE" ]; then

        echo "ERROR: data file is empty."

        return 1

    fi

    echo "Data is valid."

    return 0

}

generate_report() {

    echo "Step 3: Generating report..."

    wc -l "$DATA_FILE" > "$RESULT_FILE"

    echo "Report created in $RESULT_FILE"

}

download_data &

PID_DOWNLOAD=$!

echo "Waiting for download process to finish..."

wait $PID_DOWNLOAD

validate_data

STATUS_VALID=$?

if [ $STATUS_VALID -eq 0 ]; then

    generate_report

    echo "Workflow completed successfully."

    echo "Report content:"

    cat "$RESULT_FILE"

else

    echo "Workflow failed because validation did not pass."

fi
```

### Practice Steps

1. Save it as `task5_process_dependency.sh`
2. Make it executable:

```bash
chmod +x task5_process_dependency.sh
```

3. Run it:

```bash
./task5_process_dependency.sh
```

### Script Explanation

- `download_data` simulates a data collection process.
- This process is run in the background to show that the task can be separated.
- `wait $PID_DOWNLOAD` ensures that the next step does not start before the first process is complete.
- `validate_data` checks whether the file exists and is not empty.
- `generate_report` only runs if validation succeeds.

### What to Observe

- The second process depends on the result of the first process
- `wait` is used for dependency synchronization
- Exit status is used to determine the next step

### Expected Outcome

Students understand that dependencies between processes are very common and important. They also learn that Bash can be used to build simple, safe, and structured workflows, not just a loose collection of commands.
