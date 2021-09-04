#1/bin/bash

if [ "$#" -ne 3 ]; then
	echo "error: 3 args needed"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "error: invalid file $1"
	exit 2
fi

if ( egrep -q "${3}=" "$1" ); then
	term1="$(egrep "$2=" "$1" | cut -d '=' -f 2 | sed -r 's/ /\n/g')"
	term2="$(egrep "$3=" "$1" | cut -d '=' -f 2)"

	while read term; do
        term2="$(echo "$term2" | sed -E 's/( |^)'"$term"'( |$)/ /g' | xargs)"
	done <<<"$term1"

	sed -Ei 's/('"$3"'=).*/\1'"$term2"'/g' "$1"
fi

exit 0
