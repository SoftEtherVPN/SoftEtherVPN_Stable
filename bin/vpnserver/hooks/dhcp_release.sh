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
iptables -t nat -D POSTROUTING -s $tapip -j SNAT --to 164.132.4.206
