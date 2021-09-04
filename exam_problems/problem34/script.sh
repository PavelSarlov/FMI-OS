#!/bin/bash -x

parsed="${HOME}/exam_problems/problem34/before"
dir="${HOME}/exam_problems/problem34/temp"

touch "${parsed}"

if [ "$#" -ne 1 ]; then
    echo "error: invalid number of args" >&2
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "error: invalid directory" >&2
    exit 2
fi

find "$1" -type f 2>/dev/null |\
    egrep "^$( basename "$1" )/[^_]*_report-[0-9]+\.tgz" |\
    while read archive; do
        sha_sum_before="$( egrep " ${archive}$" "${parsed}" | cut -d ' ' -f 1)"
        sha_sum_now="$( sha256sum "${archive}" | cut -d ' ' -f 1 )"

        if ( ! egrep -q " ${archive}$" "${parsed}" ) || [ ! "${sha_sum_before}" = "${sha_sum_now}" ]; then
            # file must be parsed
            if [ ! -d "${dir}" ]; then
                mkdir "${dir}" "$( dirname "${dir}" )/extracted"
            fi

            meow="$( tar -C ${dir} -xvf ${archive} | egrep "(^|/)meow\.txt$" )"

            if [ -n "${meow}" ]; then
                # meow
                name="$( basename "${archive}" | cut -d '_' -f 1 )"
                timestamp="$( basename "${archive}" | cut -d '_' -f 2 | cut -d '.' -f 1 )"

                cp "${dir}/${meow}" "$( dirname "${parsed}" )/extracted/${name}_${timestamp}.txt"
            fi

            #sed -i "|  ${archive}|d" "${parsed}" >/dev/null
            egrep -v "  ${archive}$" "${parsed}" >$( dirname "${parsed}" )/swap
            mv $( dirname "${parsed}" )/swap "${parsed}"

            rm -r "${dir}"
            sha256sum "${archive}" >>"${parsed}"
        fi
    done
