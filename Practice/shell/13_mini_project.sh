#!/bin/bash

show_result() {
    local average="$1"

    if [ "$average" -ge 75 ]; then
        echo "Status : Passed"
    else
        echo "Status : Failed"
    fi
}

echo "Student Score Calculator"
read -p "Enter student name: " student_name

total=0
count=3

for exam in 1 2 3; do
    read -p "Enter score $exam: " score
    total=$((total + score))
done

average=$((total / count))

echo "Name   : $student_name"
echo "Total  : $total"
echo "Average: $average"
show_result "$average"
