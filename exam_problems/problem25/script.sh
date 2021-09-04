#!/bin/bash

if [ "$EUID" == "0" ]; then
	info="$( ps axh o user,rss |\
		awk '{ users[$1]+=$2; cnt[$1]++ } END { for(u in users) print u,users[u],cnt[u] }' )"

	while read -r line; do
		user="$( echo "$line" | cut -d ' ' -f 1 )"
		avg="$( echo "$line" | awk '{print $2/$3}' )"
		highest="$( ps auh o rss,pid,user |\
			egrep "$user" | sort -n | tail -n 1)"

		if [ "$( echo "$highest" | awk '{print $1}' )" -gt $((2*avg))]; then
			kill "$( echo "$highest" | awk '{print $2}' )"
		fi
	done <<<"$info"
fi
