#!/bin/bash -x

GLOBAL=0

f()
{
  GLOBAL=$(($GLOBAL + $LINENO))
}


#PS4='+ $(echo $LINENO)'

#export LD_PRELOAD=./libshell_profiler.so

/bin/echo 1234567890 $LINENO

/bin/echo 0987654321 $LINENO
