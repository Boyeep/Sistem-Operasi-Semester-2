#!/bin/bash

(sleep 5; echo "Task A completed after waiting") &

(for i in {1..5}; do
    echo "Task B is working: step $i"
    sleep 1
done) &

wait
echo "Both tasks finished"