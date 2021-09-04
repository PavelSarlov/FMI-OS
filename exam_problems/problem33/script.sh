#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "error: missing arguments"
    exit 1
fi

var=10

while [ "$#" -gt 0 ]; do

    if [[ "$1" =~ "-n" ]]; then
        shift
        if [[ ! "$1" =~ ^[[:digit:]]$ ]]; then
            echo "error: invalid variable"
            exit 2
        fi

        var="$1"
        shift
        continue
    fi

    IDF="$( echo "$1" | cut -d '.' -f 1 )"
    tail -n $var -q $1 |\
        sed -r 's/^([^ ]* [^ ]*) (.*)$/\1 '$IDF' \2/g'
    shift

done |\
    sort -n -t '-' -k 1 -k 2 -k 3 -k 3,4 -k 3,7 -k 3,10

exit 0
