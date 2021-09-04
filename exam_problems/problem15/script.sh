#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "incorrect number of arguments"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "$1 is not a valid directory!"
	exit 2
fi

# 1)
find -L "$1" -type l

# 2)
#find "$1" -type l | xargs cat 2>&1 1>/dev/null | egrep ": No such" | cut -d : -f 2 | cut -c 2-

# 3)
#find "$1" -type l | xargs file | egrep "broken sym" | cut -d : -f 1

# 4)
#find "$1" -type l -exec stat -c "%N" {} \; | while read line; do
#	link_name="$( echo $line | cut -d "'" -f 2 )"
#	link_target="$( echo $line | cut -d "'" -f 4 )"
#
#	if [ ! -e "$(dirname $link_name)/$link_target" ]; then
#		echo "$link_name"
#	fi
#done
