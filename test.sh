#!/bin/bash

#export LD_PRELOAD=./libshell_profiler.so

export LINENO

/bin/echo 1234567890 $LINENO

env
/bin/echo 0987654321 $LINENO
env
