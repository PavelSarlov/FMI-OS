#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "error: 1 argument needed"
	exit 1
elif [[ "$1" =~ "^[0-9]*$" ]]; then
	echo "error: invalid argument"
	exit 2
fi

if [ "$EUID" == "0" ]; then
	users="$(ps axh o user,pid,rss |\
		egrep -v "root " | sort |\
		awk 'BEGIN {user[""]=0;} {user[$1]+=$2} END {delete user[""]; for(i in user) print i,user[i]}'|\
		awk '{if($2>'$1') print $1}')"

	for user in "$users"; do
		pid="$(ps axh o rss,pid,user | egrep " $user$" | sort -n | tail -n 1 | awk '{print $2}')"
		kills -15 "$pid"
	done
fi

exit $?	
