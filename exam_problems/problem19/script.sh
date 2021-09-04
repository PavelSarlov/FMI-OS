#!/bin/bash

if [ $# -ne 2 ]; then
    echo "invalid num of args"
    exit 1
fi

if [ ! -f "$1" ] || [ ! -f "$2" ]; then
    echo "invalid file"
    exit 2
fi

file1="$1"
file2="$2"

lines1=$(grep $file1 $file1 | wc -l)
lines2=$(grep $file2 $file2 | wc -l)

winner=$file1

if [ $lines1 -lt $lines2 ]; then
	winner=$file2
fi

cut -d ' ' -f 4- $winner | sort >$winner".songs"

exit 0
