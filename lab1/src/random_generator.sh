#!/bin/bash

MAXCOUNT=150
count=1

#echo "-----------------"
while [ "$count" -le $MAXCOUNT ]
do
  #echo $RANDOM >> numbers.txt
  echo $(od -A n -t d -N 1 < /dev/random) >> numbers5.txt
  let "count += 1"
done
#echo "-----------------"