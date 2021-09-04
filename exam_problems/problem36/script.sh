#!/bin/bash

if [ $# -ne 1 ]; then
    echo "invalid num of args"
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "invalid file $1"
    exit 2
fi

log="$1"
topsites="$( cut -d ' ' -f 2 "$log" | sort | uniq -c | sort -nr | head -n 3 | awk '{print $2}')"

while read site; do
    http2="$( cat "${log}" | egrep "HTTP/2\.0" | cut -d ' ' -f 2 | egrep "^${site}$" | wc -l )"
    other="$( cat "${log}" | egrep "HTTP/[^2]" | cut -d ' ' -f 2 | egrep "^${site}$" | wc -l )"
    topclients="$( cat "${log}" |\
        cut -d ' ' -f 1,2,9 |\
        egrep " ${site} " |\
        awk '{if($3>302) print $1}' |\
        sort | uniq -c | sort -nr | head -n 5 )"

    echo "$site HTTP/2.0: $http2 non-HTTP/2.0: $other"
    if [ ! -z "$topclients" ]; then
        echo "$topclients"
    fi
done <<<"$topsites"

exit 0
