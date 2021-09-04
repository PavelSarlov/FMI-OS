#!/bin/bash

if [ "$#" -ne 3 ]; then
	echo "error: invalid number of arguments"
	exit 1
elif [ ! -d "$1" ]; then
	echo "error: invalid directory "$1""
	echo "usage: $0 SRC DST STR"
	exit 2
elif [ ! -d "$2" ]; then
	echo "error: invalid directory "$2""
	echo "usage: $0 SRC DST STR"
	exit 2
fi

src="$1"
dst="$2"
str="$3"

if [ "$UID" == "0" ]; then
	src="$( echo "$src" | sed -r 's/(.*)\//\1/g')"
	dst="$( echo "$dst" | sed -r 's/(.*)\/$/\1/g' )"

	while read -r line; do
		depth="$( echo "$line" | awk '{print $1}' )"
		file="$( echo "$line" | cut -d ' ' -f 2- )"
		path="$( dirname "$file" )"
		dstPath="$( echo "$path" | sed -r 's/^'"$src"'/'"$dst"'/g' )" 

		mkdir -p "$dstPath" 2>/dev/null
		mv "$file" "$dstPath"

		while [ "$path" != "$src" ]; do
			if [ "$( ls -A "$path" )" ]; then
				break
			fi

			rmdir "$path"
			path="$( dirname "$path" )"
		done
	done<<<"$( find "$src" -type f -name "*${str}*" -printf "%d %p\n" 2>/dev/null )"
fi

exit 0
