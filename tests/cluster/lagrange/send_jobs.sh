#!/bin/bash

for i in $(/bin/ls -1 | grep -v $0)
do
    cat $i
done