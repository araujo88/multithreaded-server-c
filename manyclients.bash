#!/bin/bash

for N in {1..50}
do
    ./client &
done
wait