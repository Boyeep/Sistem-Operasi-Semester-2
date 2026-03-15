#!/bin/bash

greet_user() {
    local name="$1"
    echo "Hello, $name. Keep practicing shell scripting."
}

greet_user "Student"
