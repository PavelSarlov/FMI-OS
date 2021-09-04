#! /bin/bash

egrep "^[^a-w]*[02468][^a-w]*$" "philip-j-fry.txt" | wc -l | xargs -I {} echo "Броят на търсените редове е {}"
