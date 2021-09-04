#~/bin/bash

find / 2>/dev/null |\
    xargs stat -c "%u\n" 2>/dev/null |\
    grep $(id -u) | wc -l
