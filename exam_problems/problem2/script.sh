#!/bin/bash

if [ $# -ne 1 ]; then
    echo "invalid num of args"
    exit 1
fi

file="$1"

sed -r 's/[0-9]{4} г. - //g' "$file" |\
    awk 'BEGIN {i=1} {printf("%d. ", i++); print $0;}' |\
    sort -t ' ' -k 2

exit 0
