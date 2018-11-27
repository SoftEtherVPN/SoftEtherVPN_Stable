#!/bin/bash
#echo "SE_ERROR";

username=""
meta=""
while [ "$1" != "" ]; do
	case $1 in
		-u | --username )
			shift
			username=$1
			;;
		-m | --meta )
			shift
			meta=$1
			;;
		* )
		exit 1
	esac
	shift
done
echo $username
echo "fdafda"
