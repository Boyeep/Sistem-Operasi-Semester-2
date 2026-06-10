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