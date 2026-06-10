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