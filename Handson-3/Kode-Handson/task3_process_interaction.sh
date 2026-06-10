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