#!/bin/bash

MAXCOUNT=150
count=1

echo "-----------------"
while [ "$count" -le $MAXCOUNT ]
do
  number=$RANDOM
  #echo $number
  echo $number >> numbers.txt
  let "count += 1"
done
echo "-----------------"