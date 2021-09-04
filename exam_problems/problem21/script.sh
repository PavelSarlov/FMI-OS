#1/bin/bash

if [ "$#" -ne 1 ]; then
	echo "error: invalid argument"
	exit 1
fi

if [ "$EUID" = "$ROOT_ID" ]; then
	cnt="$(ps -eho user | egrep -c "$1")"
	ps -eho user | sort | uniq -c | awk '{ if($1>'"$cnt"') print $2}'

	avg="$(ps -eho times | awk 'BEGIN {cnt=0; sum=0} {cnt++; sum+=$1} END {print sum/cnt}')"
    echo "Average time for all processes: $avg"

	ps -eho user,pid,times | egrep "$1" |\
		awk '{if($3>'"$avg"'*2) print $2}' |\
		xargs kill
fi

exit 0
