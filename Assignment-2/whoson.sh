#!/bin/bash
IFS=',' read -r -a all_array <<< "$(getent group openstack_ugrad)"

all_online=($(who |awk '{print $1}'| sort -u))

IFS=$'\n' sorted=($(sort <<<"${all_array[*]}"))

for student in "${sorted[@]:1}"; do
    for people in "${all_online[@]}"; do
       if [ $student == $people ];
        then echo -n $student 
			echo -n " " 
        fi
done
done
echo " "
