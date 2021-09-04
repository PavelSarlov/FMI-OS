#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "error: invalid number of args"
	echo "usage: $0 DIR STR"
	exit 1
elif [ ! -d "$1" ]; then
	echo "error: invalid directory $1"
	exit 2
fi

files="$( find "$1" -maxdepth 1 -type f -printf "%f\n" |\
	egrep "^vmlinuz-[0-9]*\.[0-9]*\.[0-9]*-$2$" )"

echo "$files" |\
	egrep "$( echo "$files" | cut -d '-' -f 2 | sort -t '.' -k 1n -k 2n -k 3n | tail -n 1 )"
