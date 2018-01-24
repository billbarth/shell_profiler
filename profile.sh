#!/bin/bash

## export LINENO
## 
## trap 'echo "local: $LINENO" >> /tmp/lineno_test.txt'  DEBUG
## 
## for i in $(seq 0 10); do
##   /bin/echo $i
## done

# Assumes that SP_OUTFILE is set. Test for this later.

## if ! echo $SP_OUTFILE | grep -q '^/'; then
##   echo "SP_OUTFILE must be an absolute path"
## else
##   export SP_OUTFILE=`pwd`/sp.yaml
##   echo "using SP_OUTFILE=$SP_OUTFILE instead"
## fi

# Actually required to export
export LD_PRELOAD=/home1/00564/bbarth/snippets/shell_profiler/libshell_profiler.so

echo Running: "$*"

$*

unset LD_PRELOAD

while [ ! -s $SP_OUTFILE ]; do
  date
done

#no longer necesary
# awk 'BEGIN{ print "{" } {print} END{ print "}"}' $SP_OUTFILE > tmp && mv tmp $SP_OUTFILE



