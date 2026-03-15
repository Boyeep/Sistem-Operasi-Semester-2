#!/bin/bash

temp_dir="$HOME/temp_practice"
log_file="$HOME/temp_practice_cleanup.log"
timestamp="$(date '+%Y-%m-%d %H:%M:%S')"

mkdir -p "$temp_dir"
find "$temp_dir" -type f -name '*.tmp' -delete

echo "[$timestamp] Deleted .tmp files in $temp_dir" >> "$log_file"
echo "Cleanup finished."
