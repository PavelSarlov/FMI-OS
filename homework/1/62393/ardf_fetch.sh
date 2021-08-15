#!/bin/bash

# check whether only one file is given
# and it's actually a readable file
if [ "$#" -ne 1 ]; then 
	echo "error: invalid number of arguments"
	echo "usage: $0 <file>"
	exit 1
elif [ ! -r "$1" ]; then
	echo "error: invalid file '$1' or missing reading permission"
	exit 2
fi
	
# iterate through each url
while read -r url; do

	# if line is comment or url is broken skip
	if ( echo "$url" | egrep -q "^\#.*" ) || ! ( curl -s "$url" 1>/dev/null ); then
		continue
	fi

	# whole html page
	html="$(curl -s "$url")"

	# extract date
	date="$( echo "$html" |\
		egrep "class=\"Title2 TI21" |\
		sed -r 's/.*>(.*)&.*/\1/g' |\
		sed -r 's/[[:space:]]//g' |\
		sed -r 's/(\.|^)([0-9])(\.|$)/\10\2\3/g' )"
	
	# get an array of categories
	categories=($( echo "$html" |\
		egrep "class=\"Title2 TI24" |\
		sed -r 's/<[^>]*>([^<]*)/\1\n/g' |\
		head -n -4 | tail -n +3 ))

	# get an array of row numbers
	# corresponding to each category table
	tableRows=($( echo "$html" |\
		egrep "class=\"Title2 TI25" |\
		sed -r 's/<[^>]*>([^<]*)/\1\n/g' |\
		head -n -4 | tail -n +3 ))

	# initialize starting and ending row
	sRow=1;
	eRow=0;

	# iterate through each category
	# and extract the participants in range [sRow,eRow]
	for (( i=0; i<"${#tableRows[@]}"; i++ )); do

		# set ending row to previous plus current count
		eRow=$(( eRow + tableRows[i] ))

		#extract participants
		participants="$( echo "$html" |\
			egrep "class=\"ResLine" |\
			head -n "$eRow" | tail -n +"$sRow" |\
			sed -r 's/.*<td class="pl">([^.<]*)[.]?<td class="name">(.*)<td class="clu?b">([^<]*)<td class="call">([^<]*)<td class=".*runT">([^<]*)<td class="fox">([^<]*)<td class="stno">([^<]*).*/\1:\2:\3:\4:\5:\6:\7/g' |\
			sed -r 's/&[^:]*//g' |\
			sed -r 's/^- //g' |\
			sed -r 's/([^:]*), ([^:]*)/\2 \1/g' |\
			sed -r 's/<[^>]*>//g' |\
			sed -r 's/(.*)/'${date}':'${categories[$i]}':\1/g' )" 
		
		#set starting row to previous plus current count
		sRow=$(( sRow + tableRows[i] ))

		echo "$participants"
	done
done <"$1"
