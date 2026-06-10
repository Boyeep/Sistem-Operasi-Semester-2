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