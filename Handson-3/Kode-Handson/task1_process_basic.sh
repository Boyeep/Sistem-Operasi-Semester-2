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