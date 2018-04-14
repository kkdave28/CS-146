#!/bin/bash

echo "You start with $# positional parameters"

for var in "$@"
do
	chmod +x "$var"
done
	
