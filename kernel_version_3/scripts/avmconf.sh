#!/bin/sh
if [ -z "$1" ] ;then
	avm conf

elif [ "$1" == "b" ] ;then
	avm conf

elif [ "$1" == "f" ] ; then
	if [ -z "$2" ] ;then
		avm conf
	else
		avm conf
	fi
fi
