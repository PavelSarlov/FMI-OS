#!/bin/bash

if [ $# -ne 3 ]; then
    echo "invalid num of args"
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "invalid fail $1"
    exit 2
fi

cfg="$1"
key="$2"
value="$3"

if ( ! egrep -q "^ *${key} *=" "$cfg" ); then
    echo "${key} = ${value} # added at $(date) by $(whoami)" >>"${cfg}"
elif ( ! egrep "^ *${key} *=" "$cfg" | egrep -q "= *${value} *" ); then
    sed -i -E 's/^( *'"${key}"' *.*)/# \1 # edited at '"$(date)"' by '"$(whoami)"'\n'"${key}"' = '"${value}"' # added at '"$(date)"' by '"$(whoami)"'/g' "${cfg}"
fi

exit 0
