#!/bin/bash
echo repeat_args

counter=1
while [[ "$1" != "" ]]
do
    echo $counter: $1
    shift
    counter=$((counter+1))
done
