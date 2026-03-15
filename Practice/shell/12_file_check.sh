#!/bin/bash

target="${1:-.}"

if [ -f "$target" ]; then
    echo "$target is a file."
elif [ -d "$target" ]; then
    echo "$target is a directory."
else
    echo "$target does not exist."
fi
