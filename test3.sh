#!/bin/bash

#export LD_PRELOAD=./libshell_profiler.so

N=15
NB=$((1024*1024*100))

for i in $(seq 0 $N); do
  s=$(($RANDOM % 3 ))
  echo "----- $s $i $NB -----"
  /bin/dd if=/dev/zero of="foo.$i" bs=$NB count=$(($s +1))
done
