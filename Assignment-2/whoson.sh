IFS=', ' read -r -a all_array <<< "$(getent group openstack_ugrad)"
#IFS=', ' read -r -a all_array <<< "$(getent group ugrad)"
all_online=($(who | awk '{print $1}'))
IFS=$'\n' sorted=($(sort <<<"${all_array[*]}"))
#echo "${sorted[@]:1}"

#echo "${all_online[@]:1}"
#echo ${Array1[@]} ${Array2[@]} | tr ' ' '\n' | sort | uniq -u
for student in "${all_online[@]}"; do
    for people in "${sorted[@]:1}"; do
       if [ $student == $people ];
        then echo $student 
        fi
done
done
#for element in "${array[@+1]}"
#do
 #   echo "$element"
#done
