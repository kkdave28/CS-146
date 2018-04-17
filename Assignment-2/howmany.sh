#!/bin/bash
IFS=' ' read -r -a all_array <<< $("./whoson.sh") 
echo ${#all_array[@]}   