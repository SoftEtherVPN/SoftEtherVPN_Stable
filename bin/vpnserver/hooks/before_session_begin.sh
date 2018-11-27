#!/bin/bash
#echo "SE_ERROR";

username=""
ip=""
while [ "$1" != "" ]; do
	case $1 in
		-u | --username )
			shift
			username=$1
			;;
		-ip | --remoteip )
			shift
			ip=$1
			;;
		* )
		exit 1
	esac
	shift
done
echo $username
echo $ip
