#!/usr/bin/expect

set arg1 [lindex $argv 1]
set arg2 [lindex $argv 2]
set arg3 [lindex $argv 3]
set arg4 [lindex $argv 4]
set arg5 [lindex $argv 5]
set timeout 20

set user "root"

set password "root"


spawn ssh root@192.168.122.233 "$arg1 $arg2 $arg3 $arg4 $arg5"
expect "password:"
send "$password\r";
interact
#ssh root@192.168.122.233 "$1 $2 $3 $4 $5 $6 $7 $8 $9 $10"
