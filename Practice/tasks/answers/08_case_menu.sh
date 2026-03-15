#!/bin/bash

read -p "Choose topic [nav/perm/edit]: " topic

case "$topic" in
    nav)
        echo "pwd ls cd"
        ;;
    perm)
        echo "chmod chown ls -l"
        ;;
    edit)
        echo "nano vim"
        ;;
    *)
        echo "Unknown topic"
        ;;
esac
