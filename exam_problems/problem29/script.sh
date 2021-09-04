#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "error: exactly one argument needed"
	echo "usage: "$0" LOGDIR"
	exit 1
elif [ ! -d "$1" ]; then
	echo "error: invalid directory $1"
	exit 2
fi

directs="$( find -L "$1" -mindepth 3 -maxdepth 3 -type d -print )"
declare -A friends

while read -r dir; do
	name="$( basename "$dir" )"

	friends["$name"]="$(( friends["$name"] + $( find -L "$dir" -type f | xargs cat | wc -l) ))"
done <<<"$directs"

for f in ${!friends[@]}; do
	results+="$( echo ":${friends[$f]} $f" )"
done

echo "$results" | sed -r 's/:/\n/g' | tail -n +2 |\
	sort -nr | head
