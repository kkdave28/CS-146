#!/bin/bash 
m=$#

for((i=0; i<=m; i++)); 
    do
    echo "$i : \"${!i}\""
done