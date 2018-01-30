#!/bin/bash

# Next three lines need to go in the top of any profiled script
# KSH probably requires something like BS=${.sh.file}

export LN
export BASH_SOURCE

trap ' export LN=$LINENO; export BS=$BASH_SOURCE'  DEBUG

for i in $(seq 0 10); do
  /bin/ls genv.c
  /bin/ls genv.c 
done
