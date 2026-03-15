#!/bin/bash

target_dir="${1:-.}"

echo "Current directory : $(pwd)"
echo "Target directory  : $target_dir"

if [ -d "$target_dir" ]; then
    echo "Files inside $target_dir:"
    ls -lah "$target_dir"
else
    echo "Directory not found."
fi
