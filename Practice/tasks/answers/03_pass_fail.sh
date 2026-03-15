#!/bin/bash

read -p "Enter score: " score

if [ "$score" -ge 75 ]; then
    echo "Pass"
else
    echo "Fail"
fi
