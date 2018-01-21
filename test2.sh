#!/bin/bash

#export LD_PRELOAD=./libshell_profiler.so

export LINENO

N=15

for i in $(seq 0 $N); do
  s=$(($RANDOM % 3 ))
  echo $s
  /bin/sleep $s
done
