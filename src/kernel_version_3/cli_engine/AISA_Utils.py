from __future__ import print_function
import os
import sys
import subprocess
import logging
import logging.handlers
import time
import fcntl
import struct
import fcntl
from os.path import basename

from subprocess 	import *
from AISA_Defaults      import *


def Syslog(str):
	logger = logging.getLogger('MyLogger')
	logger.setLevel(logging.DEBUG)
	handler = logging.handlers.SysLogHandler(address = '/dev/log')
	logger.addHandler(handler)
	logger.critical(str)

class Utils():
	def	__init__(self):
		self.data = []
		#print('Class ' + __name__ + ' defined')

	@staticmethod
	def currentTime():
		import time
		return time.strftime("%X")

def LOG(lvl, strarg):
	#print("AVERMA:lvl="+str(lvl)+" LOG_LEVEL:"+str(LOG_LEVEL))
	if lvl <= LOG_LEVEL:
		lvlstr = LOG_DICT[ lvl ]
		logstr = "[" + Utils.currentTime() + "][" + lvlstr + "] "+ strarg

		if lvl >= 0:
			print(logstr)

		#logstrk = "[AVM][CLILOG][" + lvlstr + "] "+ strarg
		if lvl == -1:
			logstrk = "[AVM][CLICMD] "+ strarg
		else:
			logstrk = "[AVM][CLILOG] "+ strarg
		cmdstr = "echo '" + logstrk + "' > /dev/kmsg"
		os.system(cmdstr)
		#Syslog(str)

def Log(lvl, str):
	LOG(lvl, str)

def log(lvl, str):
	LOG(lvl, str)

def LOGR(strarg):
	print(strarg)

def LOGRAW(strarg):
	print(strarg)

class ReturnInfo():
	def __init__(self, ret, retstr):
		self.ret = ret
		self.retcode = ret
		self.retstr = retstr.strip()

	def LOGRAW(self):
			print(self.retstr)

	def Log(self):
		if self.ret == 0:
			LOG(DEBUG, 'CMD Return Value  : '+ str(self.ret))
			LOG(DEBUG, 'CMD Output String : '+ self.retstr)
		else:
			LOG(DEBUG, 'CMD Return Value  : '+ str(self.ret))
			LOG(DEBUG, 'CMD Output String : '+ self.retstr)

	def log(self):
		self.Log()

	def LOG(self):
		self.Log()

class ShellCmd():
	def	__init__(self):
		self.data = []
		#print('Class ' + __name__ + ' defined')

	@staticmethod
	def run(cmdstr):
		LOG(DEBUG,"CMD : "+ cmdstr)
		return os.system(cmdstr)

	@staticmethod
	def runo(cmdstr):
		LOG(DEBUG,"CMD: "+ cmdstr)
		proc = subprocess.Popen(cmdstr, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		outstr = proc.stdout.read()
		outstr = outstr + proc.stderr.read()
		ret = proc.wait()
		retobj = ReturnInfo(ret, outstr)
		return retobj

	@staticmethod
	def runr(cmdstr):
		proc = subprocess.Popen(cmdstr, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		outstr = proc.stdout.read()
		outstr = outstr + proc.stderr.read()
		ret = proc.wait()
		retobj = ReturnInfo(ret, outstr)
		return retobj

	@staticmethod
	def runr(cmdstr): #Raw run i.e. without any debug messages
		proc = subprocess.Popen(cmdstr, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		outstr = proc.stdout.read()
		outstr = outstr + proc.stderr.read()
		ret = proc.wait()
		retobj = ReturnInfo(ret, outstr)
		return retobj

	@staticmethod
	def load_module(module):
		knamebase = ShellCmd.basename(module)
		kname = os.path.splitext(knamebase)[0]

		cmdstr = "cp " + module + " /lib/modules/`uname -r`/extra/ "
		retobj = ShellCmd.runo(cmdstr)
		retobj.LOG()
		if retobj.ret == 0:
			cmdstr = "depmod -a "
			retobj = ShellCmd.runo(cmdstr)
			retobj.LOG()
			if retobj.ret == 0:
        			cmdstr = "/usr/sbin/modprobe " + kname + " > /dev/null 2>&1"
				retobj = ShellCmd.runo(cmdstr)
				retobj.LOG()
				return SUCCESS

	@staticmethod
	def insmod(module):
        	cmdstr = "insmod " + module + " > /dev/null 2>&1"
	        return ShellCmd.run(cmdstr)

	@staticmethod
	def remove_module(module):
		knamebase = ShellCmd.basename(module)
        	cmdstr = "rmmod " + knamebase + " > /dev/null 2>&1"
	        return ShellCmd.runo(cmdstr)

	@staticmethod
	def date():
	        return ShellCmd.run('date')

	@staticmethod
	def basename(path):
		return os.path.basename(path)

class FileUtils():
	def	__init__(self):
		self.data = []
		#print('Class ' + __name__ + ' defined')

	@staticmethod
	def isFile(fname):
		return os.path.isfile(fname)

	@staticmethod
	def isDir(fname):
		return os.path.exists(fname)


def getPackagePath():
	print (sys.path)
	return sys.path

def addPath(arg_path):
	sys.path.append(arg_path)

def getSizeBytes(device):
	cmdstr = "blockdev --getsize64 " + device # Return size in bytes
	return ShellCmd.runo(cmdstr)

def getSizeBlocks(device): 
	cmdstr = "blockdev --getsz " + device # Return size in 512 bytes block
	return ShellCmd.runo(cmdstr)

def getUUID():
	cmdstr = "cat /proc/sys/kernel/random/uuid"
	return ShellCmd.runo(cmdstr)

def getHex(ch):
	return ch.encode('hex')

def Trim(arg_str):
	return arg_str.strip()

def trim(arg_str):
	return Trim(arg_str)

def getCurrentDate():
	return time.strftime("%d/%m/%Y")

def getCurrentTime():
	return time.strftime("%H:%M:%S")

def getCurrentTimeAndDate():
	return (getCurrentTime() + " " + getCurrentDate())

def getCurrentDateAndTime():
	return (getCurrentDate() + " " + getCurrentTime())

def getCurrentDateRaw():
	return time.strftime("%d%m%Y")

def getCurrentTimeRaw():
	return time.strftime("%H%M%S")

#Input is String but function is capable of handling string and integer both
#Output is String
def convert_bytes_to_blocks(arg_size_in_bytes):
	BS = 512
	#print (type(arg_size_in_bytes))
	if (type(arg_size_in_bytes) == int):
		size_in_bytes = (arg_size_in_bytes)
		#LOG(DEBUG, "Size " + str(arg_size_in_bytes) + " in integer format")

	if (type(arg_size_in_bytes) == str):
		size_in_bytes = (int(arg_size_in_bytes))
		#LOG(DEBUG, "Size " + arg_size_in_bytes + " in string format")

	size_in_blocks = (size_in_bytes / BS)
	return str(size_in_blocks)

#Input is String but function is capable of handling string and integer both
#Output is String
def convert_bytes_to_kb(arg_size_in_bytes):
	KB = 1024
	#print (type(arg_size_in_bytes))
	if (type(arg_size_in_bytes) == int):
		size_in_bytes = (arg_size_in_bytes)
		#LOG(DEBUG, "Size " + str(arg_size_in_bytes) + " in integer format")

	if (type(arg_size_in_bytes) == str):
		size_in_bytes = (int(arg_size_in_bytes))
		#LOG(DEBUG, "Size " + arg_size_in_bytes + " in string format")

	size_in_kb = (size_in_bytes / KB)
	return str(size_in_kb)

#Input is String but function is capable of handling string and integer both
#Output is String
def convert_bytes_to_mb(arg_size_in_bytes):
	MB = 1024 * 1024
	#print (type(arg_size_in_bytes))
	if (type(arg_size_in_bytes) == int):
		size_in_bytes = (arg_size_in_bytes)
		#LOG(DEBUG, "Size " + str(arg_size_in_bytes) + " in integer format")

	if (type(arg_size_in_bytes) == str):
		size_in_bytes = (int(arg_size_in_bytes))
		#LOG(DEBUG, "Size " + arg_size_in_bytes + " in string format")

	size_in_mb = (size_in_bytes / MB)
	return str(size_in_mb)

#Input is String but function is capable of handling string and integer both
#Output is String
def convert_bytes_to_gb(arg_size_in_bytes):
	GB = 1024 * 1024 * 1024
	#print (type(arg_size_in_bytes))
	if (type(arg_size_in_bytes) == int):
		size_in_bytes = (arg_size_in_bytes)
		#LOG(DEBUG, "Size " + str(arg_size_in_bytes) + " in integer format")

	if (type(arg_size_in_bytes) == str):
		size_in_bytes = (int(arg_size_in_bytes))
		#LOG(DEBUG, "Size " + arg_size_in_bytes + " in string format")

	size_in_gb = (size_in_bytes / GB)
	return str(size_in_gb)

#Input is String but function is capable of handling string and integer both
#Output is String
def convert_gb_to_blocks(arg_size_in_gb):
	GB = 1024 * 1024 * 1024
	BS = 512
	#print (type(arg_size_in_gb))
	if (type(arg_size_in_gb) == int):
		size_in_bytes = (arg_size_in_gb * GB)
		#LOG(DEBUG, "Size " + str(arg_size_in_gb) + " in integer format")

	if (type(arg_size_in_gb) == str):
		size_in_bytes = (int(arg_size_in_gb) * GB)
		#LOG(DEBUG, "Size " + arg_size_in_gb + " in string format")

	size_in_blocks = (size_in_bytes / BS)
	return str(size_in_blocks)

#Input is String but function is capable of handling string and integer both
#Output is String
def convert_mb_to_blocks(arg_size_in_mb):
	MB = 1024 * 1024
	BS = 512
	#print (type(arg_size_in_mb))
	if (type(arg_size_in_mb) == int):
		size_in_bytes = (arg_size_in_mb * MB)
		#LOG(DEBUG, "Size " + str(arg_size_in_mb) + " in integer format")

	if (type(arg_size_in_mb) == str):
		size_in_bytes = (int(arg_size_in_mb) * MB)
		#LOG(DEBUG, "Size " + arg_size_in_mb + " in string format")

	size_in_blocks = (size_in_bytes / BS)
	return str(size_in_blocks)

def get_block_device_size(device_name):
	##f = open(device_name)
	#with open(device_name, 'r') as fd:
	#	#fcntl.ioctl(fd, fcntl.BLKGETSIZE, &numblocks)
	#	bs = fcntl.ioctl(fd, fcntl.BLKGETSIZE64)

	#print(bs)
	pass
