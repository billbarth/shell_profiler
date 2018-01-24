#!/bin/bash


export SP_OUTFILE=~/snippets/shell_profiler/mpi.yaml
/bin/echo Before ibrun
ibrun ~/snippets/mpihello/hello
/bin/echo After ibrun
