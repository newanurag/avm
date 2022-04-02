import sys
import os


os.system

for port in range(5000, 5050):
	cmdstr = '/usr/bin/firewall-cmd --permanent --add-port='+ str(port)+'/tcp'
	print(cmdstr)
	os.system(cmdstr)

cmdstr = '/usr/bin/firewall-cmd --reload'
print(cmdstr)
os.system(cmdstr)
