#!/bin/bash

echo "Choose a Linux command topic:"
echo "1. Navigation"
echo "2. Permission"
echo "3. Text Editor"
read -p "Enter choice [1-3]: " choice

case "$choice" in
    1)
        echo "Navigation commands: pwd, ls, cd"
        ;;
    2)
        echo "Permission commands: chmod, chown, ls -l"
        ;;
    3)
        echo "CLI editors: nano, vim"
        ;;
    *)
        echo "Invalid choice"
        ;;
esac
