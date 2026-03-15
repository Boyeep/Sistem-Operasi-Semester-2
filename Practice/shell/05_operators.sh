#!/bin/bash

a=12
b=5

echo "a = $a"
echo "b = $b"
echo "Addition       : $((a + b))"
echo "Subtraction    : $((a - b))"
echo "Multiplication : $((a * b))"
echo "Division       : $((a / b))"
echo "Modulo         : $((a % b))"

if [ "$a" -gt "$b" ]; then
    echo "$a is greater than $b"
fi
