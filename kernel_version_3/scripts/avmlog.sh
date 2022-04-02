#!/bin/sh
if [ -z "$1" ] ;then
	dzcat /dev/dzlog

elif [ "$1" == "b" ] ;then
	cat /dev/dzlog

elif [ "$1" == "f" ] ; then
	if [ -z "$2" ] ;then
		dzcat /dev/dzlog > dzlog.out
		echo "Log Output saved in dzlog.out"
	else
		dzcat /dev/dzlog > "$2"
		echo "Log Output saved in $2"
	fi
fi
