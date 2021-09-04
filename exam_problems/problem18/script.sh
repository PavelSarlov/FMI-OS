#!/bin/bash

if [ $# -ne 2 ]; then
    echo "invalid number of args"
    exit 1
fi

POS1=$1;
POS2=$2;
DIR=$(pwd);

mkdir a b c 2>/dev/null

for file in $DIR/*; do
	if ! [ -f "$file" ]; then
		continue
	fi

	lines=$(wc -l "$file" | awk '{print $1}')

	if [[ $lines -lt $POS1 ]]; then
		mv "$file" a/
	elif [[ $lines -ge $POS1 ]] && [[ $lines -le $POS2 ]]; then
		mv "$file" b/
    else
		mv "$file" c/
	fi
done
