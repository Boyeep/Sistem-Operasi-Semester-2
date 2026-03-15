#!/bin/bash

name="${1:-Guest}"
score="${2:-0}"

echo "Name  : $name"
echo "Score : $score"

if [ "$score" -ge 75 ]; then
    echo "Result: Passed"
else
    echo "Result: Failed"
fi
