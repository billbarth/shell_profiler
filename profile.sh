#!/bin/bash

# Assumes that SP_OUTFILE is set. Test for this later.

# Actually required to export
echo $SP_OUTFILE $*

export LD_PRELOAD=/home1/00564/bbarth/snippets/shell_profiler/libshell_profiler.so

set -x
export PS4='+ $(echo $LINENO > /dev/null)'

$*

set +x
export PS4='+ '


unset LD_PRELOAD

while [ ! -s $SP_OUTFILE ]; do
  date
done

#no longer necesary
# awk 'BEGIN{ print "{" } {print} END{ print "}"}' $SP_OUTFILE > tmp && mv tmp $SP_OUTFILE



