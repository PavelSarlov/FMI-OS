#!/bin/bash

if [ $# -ne 2 ]; then
    echo "error: invalid number of arguments"
    exit 1
fi

if [ ! -r "$1" ]; then
    echo "error: invalid file or missing reading permissions"
    exit 2
fi

if [ ! -d "$2" ] || [ "$(ls "$2" | wc -l)" -ne 0 ]; then
    echo "error: invalid or non-empty directory"
    exit 3
fi

dir="$(echo "$2" | sed -r 's/([^\/]*)$/\1\//g')"
file="$1"

cat $file |\
    sed -r 's/([^(]*) *\(?.*:.*/\1/g' |\
    sed -r 's/ *$//g' |\
    sort |\
    uniq |\
    awk 'BEGIN {i=0} { print $0";"++i }' >"${dir}dict.txt"

while read -r line; do
    name="$(echo "$line" | cut -d ';' -f 1)"
    num="$(echo "$line" | cut -d ';' -f 2)"

    egrep "${name}.*:" $file >"${dir}${num}.txt"
done <"${dir}dict.txt"
