#!/bin/bash
#echo "SE_ERROR";

tapip=""
tapmac=""
publicip=""
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
		--publicip )
			shift
			publicip=$1
			;;
		* )
		exit 1
	esac
	shift
done
echo 'ip:' $tapip 'mac:' $tapmac 'publicip:' $publicip
iptables -t nat -I POSTROUTING -s $tapip -j SNAT --to $publicip
