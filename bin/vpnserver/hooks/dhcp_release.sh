#!/bin/bash
#echo "SE_ERROR";

tapip=""
tapmac=""
while [ "$1" != "" ]; do
	case $1 in
		--tapip )
			shift
			tapip=$1
			;;
		--tapmac )
			shift
			tapmac=$1
			;;
		* )
		exit 1
	esac
	shift
done
echo 'ip:' $tapip 'mac:' $tapmac
