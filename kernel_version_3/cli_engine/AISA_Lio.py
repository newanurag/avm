#! /usr/bin/python
from __future__ import print_function
import os
import sys

from AISA_Utils		import *
from AISA_Defaults	import *
from AISA_Lvm		import *
from AISA_Dmtarget	import *

from rtslib		import *
from rtslib		import RTSRoot




AISA_DEV_NAME		= "/dev/vg_aisp/lv_aisp"


class AISA_LIO:
	'Common base class for AISA LIO Operations'
	

	def     __init__(self):
		self.data = []
		LOG(DEBUG, 'Class ' + __name__ + ' defined')
		self.so = None

	def createStorageObjectOnBlockDevice(self, soname, devname):
		# Check if Storage Object exists
		isExist = self.checkStorageObjectExists(soname, devname)
		if isExist == True:
			LOG(INFO, "Storage Object "+soname+" already exists for device "+devname)
			self.deleteStorageObject(soname, devname)
			return FAILURE

		self.so = BlockStorageObject(soname, devname, False, False)
		return SUCCESS

	def createTarget(self, ttype, addr, portno):

		wwn = "iqn.2003-01.org.linux-iscsi.aisa-machine.x8664:sn.68ecf3bbc59a"
		self.getTargetList("iscsi")

		if ttype == "iscsi":
			self.fo 	= fabric.ISCSIFabricModule()
			self.target	= Target(self.fo, mode='create') # Creates a default iSCSI Target
			self.tpg 	= TPG(self.target, 1)
			self.portal 	= NetworkPortal(self.tpg, addr, portno)
			#self.portal 	= NetworkPortal(tpg, "192.168.1.4", "3260")
			return self.tpg
		return None


	def createLun(self, arg_tpg, arg_bo, arg_lunNo, arg_lunName):
		tpg = arg_tpg;
		self.lun = tpg.lun(arg_lunNo, arg_bo, arg_lunName)  
		return self.lun
	

	def createNodeACLObject(self, arg_tpg, initiator):
		tpg = arg_tpg
		nodeACL = tpg.node_acl(initiator)
		return nodeACL

	def doLunMapping(self, arg_lunNo, arg_nodeACL):
		self.nodeACL = arg_nodeACL
		self.mappedLun = nodeACL.mapped_lun(arg_lunNo, 0, False)
		return mappedLun
		

	def start(self, soname, devname, lunNo, lunName, ttype, initiator, ipaddr, portno):
		ret  = self.createStorageObjectOnBlockDevice(soname, devname)
		if ret == SUCCESS:
			tpg = self.createTarget(ttype, ipaddr, portno)
			lun = self.createLun(tpg, self.so, lunNo, lunName) 

	def print(self):
		if self.so is not None:
			LOG(INFO, "Storage Object Name : " + self.getStorageObjectName())
			LOG(INFO, "Storage Object WWN  : " + self.getStorageObjectWWN())
			LOG(INFO, "Block Device Name   : " + self.getStorageObjectBlockDevice())
			LOG(INFO, "Target WWN          : " + self.getTargetWWN())
			LOG(INFO, "Lun No              : " + str(self.getLunNo()))

		LOG(INFO, "List of existing Storage Objects")
		for so in self.getStorageObjectList():
			LOG(INFO, "Storage Object Name : " + self.getStorageObjectName(so))

		LOG(INFO, "List of existing Luns")
		for lun in self.getLunList():
			LOG(INFO, "Lun No              : " + str(self.getLunNo(lun)))

		LOG(INFO, "List of existing Targets")
		for target in self.getTargetList("iscsi"):
			LOG(INFO, "Target WWN          : " + self.getTargetWWN(target))


	def getLunNo(self, lun=None):
		if lun is None:
			return self.lun._lun
		else:
			return lun._lun

	def getLunWWULN(self):
		return self.lun.wwuln

	def getTargetWWN(self, target=None):
		if target is None:
			return self.target.wwn
		else:
			return target.wwn


	def getStorageObjectName(self, so=None):
		if so is None:
			return self.so._get_name()
		else:
			return so._get_name()

	def getStorageObjectBlockDevice(self):
		return self.so._get_udev_path()

	def getStorageObjectWWN(self):
		return self.so.wwn


	def checkStorageObjectExists(self, soname, devname):
		so = self.findStorageObject(soname, devname)
		if so is not None:
			return True
		else:
			return False

	def checkTargetExists(self, wwn):
		target = self.findTarget(wwn)
		if target is not None:
			return True
		else:
			return False
		
	def getStorageObjectList(self):
		for so in RTSRoot().storage_objects:
			yield so

	def getTargetList(self, ttype):
		rtsroot = RTSRoot()

		for fm in rtsroot.fabric_modules:
			for target in fm.targets:
				yield target

	def getLunList(self):
		rtsroot = RTSRoot()
		for tpg in rtsroot.tpgs:
			for lun in tpg.luns:
				yield lun

	def findStorageObject(self, soname, devname):
		for so in RTSRoot().storage_objects:
			if so.udev_path and os.path.samefile(devname, so.udev_path):
				return so

			return None

	def findTarget(self, wwn):
		rtsroot = RTSRoot()
		for fm in rtsroot.fabric_modules:
			for target in fm.targets:
				if target.wwn == wwn:
					return target
		return None

	def deleteStorageObject(self, soname, devname):
		so = self.findStorageObject(soname, devname)
		if so is not None:
			so.delete()
			LOG(INFO, "Storage Object "+soname+" on device "+devname+" delete successfully")

	def deleteAllStorageObjects():
		rtsroot = RTSRoot()
		#for so in self.storage_objects:
		#for so in StorageObject.all()


	def deleteTarget(self, wwn):
		target = findTarget(wwn)
		if target is not None:
			target.delete()

	def deleteEntireTargetConfiguration(self):
		rtsroot = RTSRoot()
		rtsroot.clear_existing(True)
		


def main():
	LOG(INFO, "Starting LIO")
	lio = AISA_LIO()

	devname 	= "/dev/vg_aisp/lv_aisp"
	lunNo		= 121
	lunName		= "aisa_lun+"+str(lunNo)
	initiator	= "abcdefghijklmnop"
	ttype		= "iscsi"
	ipaddr		= "192.168.1.5"
	portno		= 3260
	soname 		= "aisa_backstore_"+str(lunNo)

	lio.getLunList()
	lio.start(soname, devname, lunNo, lunName, ttype, initiator, ipaddr, portno)

	lio.print()

if __name__ == "__main__":
	main()

