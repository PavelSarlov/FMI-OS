#!/bin/bash

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
	echo "error: incorrect number of args"
	echo "usage: $0 <DIR> [NUM]"
	exit 1
elif [ ! -d "$1" ]; then
	echo "error: '$1' is not a valid directory"
	exit 2
fi

if [ ! -z "$2" ]; then
	# hard links
#	find "$1" -type f -exec stat -c "%n %h" {} \; | while read line; do
#		file="$( echo "$line" | cut -d ' ' -f 1 )"
#		hardlinks_count="$( echo "$line" | cut -d ' ' -f 2 )"
#		if [ "$hardlinks_count" -ge "$2" ]; then
#			echo "$file"
#		fi
#	done

	find "$1" -type f -printf "%n %f\n" 2>/dev/null |\
        awk -v lim="$2" '$1 >= lim { print $2 }'
else
	# soft links
	find -L "$1" -type l
fi
