#!/bin/bash

show_status() {
    if ! ps -f -p "$1"; then
        echo "Process is no longer running."
    fi
}

echo "Running a sleep process in the background..."
sleep 30 &
PID_BG=$!

echo "Background process PID: $PID_BG"
echo "Displaying process status with ps:"
show_status "$PID_BG"

echo "Waiting for 3 seconds..."
sleep 3

echo "Checking process status again:"
show_status "$PID_BG"

echo "Stopping the process..."
kill "$PID_BG"

echo "Status after kill:"
show_status "$PID_BG"
