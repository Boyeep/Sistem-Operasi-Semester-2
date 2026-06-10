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