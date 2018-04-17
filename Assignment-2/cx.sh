#!/bin/bash
if [[ "$#" == 0 ]] 
	then 
	echo "No Arguments Detected. Usage: ./cx.sh <arg-1> <arg-2> <arg-3> ..."
else
	for var in "$@"
		do
			chmod +x "$var"
	done
fi