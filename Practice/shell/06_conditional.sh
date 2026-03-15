#!/bin/bash

read -p "Enter your score: " score

if [ "$score" -ge 85 ]; then
    echo "Grade: A"
elif [ "$score" -ge 70 ]; then
    echo "Grade: B"
elif [ "$score" -ge 60 ]; then
    echo "Grade: C"
else
    echo "Grade: D"
fi
