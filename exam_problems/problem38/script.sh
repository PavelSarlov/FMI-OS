#!/bin/bash

if [ $# -ne 2 ]; then
    echo "invalid num of args" >&2
    exit 1
fi

SRC="$( echo "$1" | sed -E 's/\/*^//g' )"
DST="$( echo "$2" | sed -E 's/\/*^//g' )"

if [ ! -d "$SRC" ]; then
    echo "invalid directory $SRC" >&2
    exit 2
fi

find "$SRC" -type f 2>/dev/null |\
    egrep ".jpg$" |\
    while read file; do

        title="$( basename "$file" |\
            sed -E 's/(.*)\.jpg$/\1/g' |\
            sed -E 's/\([^\)]*\)//g' |\
            tr -s ' ' |\
            sed -E 's/^ *([^ ]*)/\1/g' |\
            sed -E 's/([^ ]*) *$/\1/g' )"
        album="$( basename "$file" |\
            sed -E 's/.*\(([^\)]*)\)/\1/g' )"
        date="$( stat --format="%y" "$file" |\
            cut -d ' ' -f 1 )"
        hash="$( sha256sum "$file" | cut -c 1-16 )"

        if [ -z "$album" ]; then
            album="misc"
        fi

        dat="${DST}/by-date/${date}/by-album/${album}/by-title/${title}"
        dt="${DST}/by-date/${date}/by-title"
        adt="${DST}/by-album/${album}/by-date/${date}/by-title"
        at="${DST}/by-album/${album}/by-title"
        t="${DST}/by-title"
        i="${DST}/images"
        filepath="$( cd "$(dirname "$file" )" && pwd )/$( basename "$file" )"

        mkdir -p "$dat" "$dt" "$adt" "$at" "$t" "$i" 2>/dev/null
        
        cp "${filepath}" "${i}/${hash}.jpg"
        ln -s "${filepath}" "${dat}/${title}.jpg"
        ln -s "${filepath}" "${dt}/${title}.jpg"
        ln -s "${filepath}" "${adt}/${title}.jpg"
        ln -s "${filepath}" "${at}/${title}.jpg"
        ln -s "${filepath}" "${t}/${title}.jpg"

    done

exit 0
