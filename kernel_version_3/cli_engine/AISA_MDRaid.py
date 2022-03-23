from __future__ import print_function
import os
import sys
import getopt
import cmd
from lsm.lsmcli.cmdline import cmds
from sys import argv
 
from AISA_AVMHelp     import *

class AVM_MDRAID():
	'Common base class for system mdraid operations'

	def	__init__(self):
		self.data = []
		str = 'Class ' + __name__ + ' defined'
		LOG(DEBUG, str)

	def assemble(self):
		self.start()

	def start_V2(self):
		disks = str(AISA_DATA_DISKS)
		cmdstr = "mdadm --assemble /dev/md0 "+disks
		return ShellCmd.runo(cmdstr)

	def start_data_disks(self):
		disks = str(AISA_DATA_DISKS)
		cmdstr = "mdadm --assemble --scan"
		LOG(INFO, "Starting MDRaid Array")
		retobj = ShellCmd.runo(cmdstr)
		retobj.Log()
		if retobj.ret == 0:
			r = self.getSize()
			s = self.getSizeBlocks()
			LOG(INFO, "MDRaid Array Size: "+r.retstr+" Bytes OR "+s.retstr+" Blocks/Sectors")
			return SUCCESS
		else:		
			return retobj.ret

	def start_metadata_disks(self):
		disks = str(AISA_METADATA_DISKS)
		cmdstr = "mdadm --assemble --scan"
		LOG(INFO, "Starting MDRaid Array")
		retobj = ShellCmd.runo(cmdstr)
		retobj.Log()
		if retobj.ret == 0:
			r = self.getSize()
			s = self.getSizeBlocks()
			LOG(INFO, "MDRaid Array Size: "+r.retstr+" Bytes OR "+s.retstr+" Blocks/Sectors")
			return SUCCESS
		else:		
			return retobj.ret


	def start(self):
		disks = str(AISA_DATA_DISKS)
		cmdstr = "mdadm --assemble --scan"
		LOG(INFO, "Starting MDRaid Array")
		retobj = ShellCmd.runo(cmdstr)
		retobj.Log()
		if retobj.ret == 0:
			r = self.getSize()
			s = self.getSizeBlocks()
			LOG(INFO, "MDRaid Array Size: "+r.retstr+" Bytes OR "+s.retstr+" Blocks/Sectors")
			return SUCCESS
		else:		
			return retobj.ret

	def stop(self):
		cmdstr = "mdadm --verbose --stop "+ AISA_DATA_DISKS_ARRAY
		LOG(INFO, "Stopping MDRaid Array")
		retobj = ShellCmd.runo(cmdstr)
		retobj.Log()
		if retobj.ret == 0:
			#return SUCCESS
			pass
		else:		
			return FAILURE

		return stop_metadata_disks()

	def stop_metadata_disks(self):
		cmdstr = "mdadm --verbose --stop "+ AISA_METADATA_DISKS_ARRAY
		LOG(INFO, "Stopping MDRaid Array for Metadata Disks")
		retobj = ShellCmd.runo(cmdstr)
		retobj.Log()
		if retobj.ret == 0:
			return SUCCESS
		else:		
			return FAILURE

	def stop2(self):
		cmdstr = "mdadm --verbose --stop /dev/md0"
		LOG(INFO, "Stopping MDRaid Array")
		return ShellCmd.runo(cmdstr)

	def stat(self):
		cmdstr = "cat /proc/mdstat"
		return ShellCmd.runo(cmdstr)

	def save(self):
		self.save_config()
	
	def save_config(self):
		cmdstr = "mdadm --verbose --detail --scan >> /etc/mdadm/mdadm.conf"
		return ShellCmd.run(cmdstr)

	def monitor(self):
		cmdstr = "mdadm --monitor"
		return ShellCmd.runo(cmdstr)
	
	#Thi will create RAID-6 array (Minimum 6 disks required)
	def create_data_array(self):
		cnt = str(AISA_DATA_DISKS_CNT)
		disks = str(AISA_DATA_DISKS)
		LOG(INFO, "Creating MDRaid Array from Data Disks of raidlevel 6 (RAID6)")
		cmdstr = "mdadm  --create "+AISA_DATA_DISKS_ARRAY +" --metadata 1.2 --level=6 --raid-devices="+cnt+" "+disks
		#cmdstr = "mdadm  --create /dev/md0 --force --metadata 1.2 --level=0 --raid-devices="+cnt+" "+disks
		return ShellCmd.runo(cmdstr)

	#Thi will create RAID-1  i.e. mirrored disk array (Minimum 2 disks required)
	def create_metadata_array(self):
		cnt = str(AISA_METADATA_DISKS_CNT)
		disks = str(AISA_METADATA_DISKS)
		LOG(INFO, "Creating MDRaid Array from Metedata Disks of RAID-1 i.e. Mirroring")
		cmdstr = "mdadm  --create "+AISA_METADATA_DISKS_ARRAY+"  --metadata 1.2 --level=1 --raid-devices="+cnt+" "+disks
		#cmdstr = "mdadm  --create /dev/md0 --force --metadata 1.2 --level=0 --raid-devices="+cnt+" "+disks
		return ShellCmd.runo(cmdstr)

	#Thi will create Linear disk array (Minimum 1 disk required)
	def create_linear_metadata_array(self):
		cnt = str(AISA_METADATA_DISKS_CNT)
		disks = str(AISA_METADATA_DISKS)
		LOG(INFO, "Creating MDRaid Array from Metedata Disks of RAID-1 i.e. Mirroring")
		cmdstr = "mdadm  --create "+AISA_METADATA_DISKS_ARRAY+"  --metadata 1.2 --level=linear --raid-devices="+cnt+" "+disks
		#cmdstr = "mdadm  --create /dev/md0 --force --metadata 1.2 --level=0 --raid-devices="+cnt+" "+disks
		return ShellCmd.runo(cmdstr)


	def getSize(self):
		cmdstr = "blockdev --getsize64 /dev/md0"
		return ShellCmd.runo(cmdstr)

	def getSizeBlocks(self):
		cmdstr = "blockdev --getsz /dev/md0"
		return ShellCmd.runo(cmdstr)
