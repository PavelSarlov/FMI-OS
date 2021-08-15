#!/bin/bash

invalid_args () {
	echo "error: invalid number of arguments"
	echo "usage: $0 <file> top_places <cat> <m> <n>"
	echo "       $0 <file> parts <name>"
	exit 2
}

# check whether given file is a readable regular file
if [ ! -r "$1" ]; then
	echo "error: invalid file '$1' or missing reading permission"
	exit 1
fi

# according to the command do the corresponding action
case "$2" in
	"top_places")

		# check number of arguments
		if [ "$#" -ne 5 ]; then
			invalid_args
		# check whether given arguments 4 and 5 are integers
		elif [[ ! "$4" =~ ^[0-9]+$ ]] || [[ ! "$5" =~ ^[0-9]+$ ]]; then
			echo "error: invalid integer given"
			exit 3
		fi

		# extract and print the required information
		cat "$1" |\
			egrep "$3" |\
			# get names with actual integers as places
			# that meet the requirements
			awk -F ':' -v place="$4" '$3 ~ /^[0-9]+$/ {if($3<=place) print $4}' |\
			sort |\
			uniq -c |\
			sort -nr |\
			# remove leading spaces
			sed -r 's/^[[:space:]]*(.*)/\1/g' |\
			head -n "$5"
		;;
	"parts")

		# check number of arguments
		if [ "$#" -ne 3 ]; then
			invalid_args
		fi

		# extract required information
		cat "$1" |\
			egrep "$3" |\
			awk -F ':' '{print $1,$2}' |\
			# sort dates by year, then month, then day
			sort -k 1,9 -k 1,5 -k 1 |\
			# map the dates to their correspoding names
			# then print and sort them
			awk '{ cat[$2] = cat[$2] ? cat[$2] ", " $1 : $1 }\
				END { for(i in cat) { print i" "cat[i] } }' OFS='' |\
			sort
		;;
	*)
		echo "error: invalid operation '$2'"
		exit 4
esac

exit 0
