#!/bin/bash
check_if_valid() {
    if [[ "$1" =~ ^[a-z][a-zA-Z0-9_]*$ ]]
    then
        echo "$1: valid"
    else
        echo "$1: invalid"
    fi
}
check_if_valid "$1"