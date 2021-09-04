#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "error: invalid number of args"
    exit 1
fi

if [ ! -d "$2" ]; then
    echo "error: invalid directory $2"
    exit 2
fi

csv="$1"
dir="$2"
heading="hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key"

touch "$csv"
echo "$heading" >"$csv"

find "$dir" -type f 2>/dev/null |\
    egrep ".*\.log$" |\
    while read file; do

        hostname="$( basename -s '.log' "$file" )"
        data="$( egrep "(: .*|license)" "$file" |\
            sed -E 's/.*: (.*)/\1/g' |\
            sed -E 's/.*has an? (.*) license./\1/g' |\
            tr '\n' ',' | sed -E 's/,$//g' )"
        echo "${hostname},${data}" >>"$csv"

    done

exit 0
