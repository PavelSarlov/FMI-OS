#!/bin/bash

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
	echo "error: invalid number of args"
	echo "usage: $0 DIR [FILE]"
	exit 1
elif [ ! -d "$1" ]; then
	echo "error: invalid directory $1"
	exit 2
elif [ "$#" -eq 2 ] && [ ! -w "$2" ]; then
	echo "error: invalid file $2 or missing writing permissions"
	exit 3
fi

sym="$( find "$1" -type l | egrep -v "$( find -L "$1" -type l )" | xargs stat -c "%N\n" )"
broken="$( find -L "$1" -type l -print | wc -l)"

if [ "$#" -eq 1 ]; then
	echo "$sym"
	echo "$broken"
else
	echo "$sym" >>"$2"
	echo "$broken" >>"$2"
fi
