#!/bin/bash

file=""
user=""
last=0

while read line; do
	cUser="$(cut -d : -f 1 <<<"$line")"
	dir="$(cut -d : -f 6 <<<"$line")"

	if [[ ! "$dir" =~ "/home" ]]; then
		continue
	fi

	found=($(find "$dir" -type f 2>/dev/null | xargs stat -c "%Y %n" 2>/dev/null | sort -n | tail -n 1))

	if [ "${#found[@]}" -eq 0 ]; then
		continue
	elif [ "${found[0]}" -gt "$last" ]; then
		user="$cUser"
		file="${found[1]}"
		last="${found[0]}"
	fi
done  <"/etc/passwd"

echo "$user -> $file"
