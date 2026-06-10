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