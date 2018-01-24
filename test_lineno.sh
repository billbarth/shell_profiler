#!/bin/bash


trap 'echo "local: $LINENO" >> /tmp/lineno_test.txt'  DEBUG

for i in $(seq 0 10); do
  /bin/echo $i
done
