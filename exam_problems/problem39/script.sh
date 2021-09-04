#!/bin/bash

if [ $# -ne 3 ]; then
    echo "invalid num of args" >&2
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "invalid file $1"
    exit 2
elif [ ! -d "$3" ]; then
    echo "invalid dir $3"
    exit 3
fi

foo="$1"
cfg="$2"
dir="$3"

find -L "${dir}" -type f 2>/dev/null |\
    egrep "\.cfg$" |\
    while read file; do

        errlines="$( egrep -vn "(^#.*|^{.*}$)" "${file}")"

        if [ -z "${errlines}" ]; then
            echo "Error in $( basename "${file}" ):"
            echo "${errlines}" | sed -E 's/(.*)/Line \1/g'
        else
            touch "$cfg"

            echo "$( cat "$file" )" >>"$cfg"

            user="$( basename -s 'cfg' "${file}" )"

            if ! ( cat "$foo" | egrep "${user}" ); then
                pass="$( pwgen 16 1 )"
                echo "${user}:$(echo "${pass}" | md5sum | cut -d ' ' -f 1)" >>"$foo"
                echo "${user} ${pass}"
            fi
        fi
    done

exit 0
