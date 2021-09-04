#!/bin/bash

FILE="./spacex.txt"

sort -n $FILE |\
    egrep "$(awk -F '|' '$3 ~ /Failure/ {print $2}' $FILE | sort | uniq -c | sort -n | awk '{print $2}' | tail -n 1 )" |\
    tail -n 1 |\
    awk -F '|' 'BEGIN {OFS=":"} {print $3,$4}'
