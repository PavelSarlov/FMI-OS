#/bin/bash

if [ "$EUID" = "$ROOT_ID" ]; then
	
	while read line; do
		if [[ ! "$line" =~ /home/* ]]; then
			echo "$line" | cut -d ':' -f 1
		else
			owner="$(echo "$line" | cut -d : -f 1)"
			dir="$(echo "$line" | cut -d : -f 6)"
			perm="$(stat -c "%a" "$dir" | sed - 's/([0-9])/\1 /g')"

			if [ "$(echo "$perm" | awk '{if($1<2 && $2<2 && $3<2) print 1; else print 0}')" -eq 1 ]; then
				echo "$line"
			fi
		fi
	done <"/etc/passwd"

	exit 0;
fi

exit 1;
