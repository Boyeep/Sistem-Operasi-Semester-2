#!/bin/bash

path_value="${1:-.}"

if [ -f "$path_value" ]; then
    echo "This is a file"
elif [ -d "$path_value" ]; then
    echo "This is a directory"
else
    echo "Path not found"
fi
