#!/bin/bash

for i in $(seq 1 5); do
  /bin/sleep "."${i} &
done

wait

