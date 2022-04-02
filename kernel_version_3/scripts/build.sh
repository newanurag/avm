#!/usr/bin/expect

set timeout 120

set user "root"

set password "root"

#spawn make clean all pkg scppkg  trm tins taisa
spawn make alld test

#expect "password:"
#expect "root@192.168.122.233's password: "
expect "root@testvm's password: "
send "$password\r";
#expect "root@192.168.122.233's password: "
expect "root@testvm's password: "
send "$password\r";
#expect "root@192.168.122.233's password: "
expect "root@testvm's password: "
send "$password\r";
#expect "root@192.168.122.233's password: "
expect "root@testvm's password: "
send "$password\r";
interact
