#!/bin/bash

echo 0 > counter.txt

increment() {
    for i in {1..100}; do
        count=$(cat counter.txt)
        count=$((count + 1))
        echo $count > counter.txt
    done
}

increment &
increment &
increment &

wait
echo "Final counter value:"
cat counter.txt