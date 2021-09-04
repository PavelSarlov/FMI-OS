#!/bin/bash

if [ "$EUID" == "0" ]; then
	users="$( ps axh o user | egrep -v "root" | sort -u )"
	rootRSS="$( ps axh o user,rss | egrep "root" | awk '{sum+=$2} END {print sum}' )"

	while read -r user; do
		dir="$( cat /etc/passwd | egrep "$user" | cut -d : -f 6 )"

		if [ ! -d "$dir" ] || [ "$user" == "$(stat -c "%U" "$dir")" ] || [ "$( stat -c "%A" "$dir" | cut -c 3 )" != "w" ]; then
			userRSS="$(ps axh o user,rss | egrep "$user" |\
				awk '{sum+=$2} END {print sum}' )"

			if [ "$userRSS" -gt "$rootRSS" ]; then
				ps axh o user,pid | egrep "$user" |\
					cut -d ' ' -f 2 | xargs kill
		fi
	done <<<"$users"
fi

exit 0
