#!/bin/bash

backup_dir="$HOME/cron_practice_backup"
log_file="$backup_dir/backup.log"
timestamp="$(date '+%Y-%m-%d %H:%M:%S')"

mkdir -p "$backup_dir"

echo "[$timestamp] Backup simulation started" >> "$log_file"
echo "[$timestamp] Important files would be copied here" >> "$log_file"
echo "[$timestamp] Backup simulation finished" >> "$log_file"

echo "Backup log updated at $log_file"
