#!/bin/bash

echo "For loop example:"
for item in shell cron awk; do
    echo "Studying topic: $item"
done

echo
echo "While loop example:"
count=1
while [ "$count" -le 3 ]; do
    echo "Iteration $count"
    count=$((count + 1))
done
